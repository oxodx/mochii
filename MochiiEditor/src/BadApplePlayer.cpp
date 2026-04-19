#include "BadApplePlayer.h"

#ifdef _WIN32
#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <shellapi.h>
#include <urlmon.h>
#endif

#include <array>
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <sstream>

namespace Mochii::BadApplePlayer {
namespace {
constexpr const wchar_t* kBadAppleRelativePath = L"assets/videos/bad_apple.mp4";
constexpr std::array<const wchar_t*, 4> kBadAppleUrls = {
    L"https://archive.org/download/BadApple_201811/Bad%20Apple%21%21.mp4",
    L"https://archive.org/download/BadApple_201811/Bad%20Apple!!.mp4",
    L"https://huggingface.co/spaces/Nick088/Bad-Apple-Video/resolve/b638ac1f80add1d288e583443f495c46224b75b3/bad-apple.mp4?download=true",
    L"https://huggingface.co/spaces/Nick088/Bad-Apple-Video/resolve/main/bad-apple.mp4?download=true"};

template <typename T>
void SafeRelease(T*& value) {
  if (value) {
    value->Release();
    value = nullptr;
  }
}

std::string ToUtf8(const std::wstring& input) {
  return std::string(input.begin(), input.end());
}

bool DownloadViaPowerShell(const std::wstring& url, const std::wstring& outPath) {
  std::ostringstream command;
  command << "powershell -NoProfile -ExecutionPolicy Bypass -Command \""
          << "$ProgressPreference='SilentlyContinue';"
          << "try { "
          << "Invoke-WebRequest -Uri '" << ToUtf8(url) << "' -OutFile '"
          << ToUtf8(outPath) << "'; exit 0 "
          << "} catch { exit 1 }\"";
  return std::system(command.str().c_str()) == 0;
}

bool DownloadViaCurl(const std::wstring& url, const std::wstring& outPath) {
  std::ostringstream command;
  command << "curl.exe -L --fail --silent --show-error \"" << ToUtf8(url)
          << "\" -o \"" << ToUtf8(outPath) << "\"";
  return std::system(command.str().c_str()) == 0;
}
}  // namespace

bool FetchIfNeeded(std::string& outPath, std::string& outMessage) {
#ifndef _WIN32
  outMessage = "Bad Apple playback is only supported on Windows right now.";
  return false;
#else
  std::filesystem::path localPath = std::filesystem::absolute(
      std::filesystem::path(kBadAppleRelativePath));
  if (!std::filesystem::exists(localPath.parent_path())) {
    std::filesystem::create_directories(localPath.parent_path());
  }

  outPath = localPath.string();
  if (std::filesystem::exists(localPath) &&
      std::filesystem::file_size(localPath) > 0) {
    outMessage = "Video already downloaded.";
    return true;
  }
  if (std::filesystem::exists(localPath)) std::filesystem::remove(localPath);

  HRESULT result = E_FAIL;
  std::string attemptedUrls;
  for (const wchar_t* url : kBadAppleUrls) {
    if (!attemptedUrls.empty()) attemptedUrls += ", ";
    attemptedUrls += ToUtf8(url);

    result = URLDownloadToFileW(nullptr, url, localPath.c_str(), 0, nullptr);
    if (SUCCEEDED(result) && std::filesystem::exists(localPath) &&
        std::filesystem::file_size(localPath) > 0)
      break;

    if (DownloadViaPowerShell(url, localPath.wstring()) ||
        DownloadViaCurl(url, localPath.wstring())) {
      result = S_OK;
      break;
    }
  }

  if (FAILED(result) || !std::filesystem::exists(localPath) ||
      std::filesystem::file_size(localPath) == 0) {
    std::ostringstream stream;
    stream << "Failed to download Bad Apple (HRESULT=0x" << std::hex << result
           << "). Tried URLs: " << attemptedUrls;
    outMessage = stream.str();
    return false;
  }

  outMessage = "Downloaded Bad Apple video.";
  return true;
#endif
}

bool LoadTileVideo(TileVideo& outVideo, std::string& outMessage,
                   uint32_t tileWidth, uint32_t tileHeight) {
#ifndef _WIN32
  (void)outVideo;
  (void)tileWidth;
  (void)tileHeight;
  outMessage = "Tilemap video decode is only supported on Windows right now.";
  return false;
#else
  std::string localPath;
  std::string fetchMessage;
  if (!FetchIfNeeded(localPath, fetchMessage)) {
    outMessage = fetchMessage;
    return false;
  }

  const HRESULT comResult = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
  if (FAILED(comResult) && comResult != RPC_E_CHANGED_MODE) {
    outMessage = "Failed to initialize COM for Media Foundation.";
    return false;
  }

  HRESULT result = MFStartup(MF_VERSION);
  if (FAILED(result)) {
    if (SUCCEEDED(comResult)) CoUninitialize();
    outMessage = "Failed to initialize Media Foundation.";
    return false;
  }

  IMFSourceReader* reader = nullptr;
  IMFAttributes* readerAttributes = nullptr;
  IMFMediaType* mediaType = nullptr;
  IMFMediaType* currentType = nullptr;

  auto shutdown = [&]() {
    SafeRelease(currentType);
    SafeRelease(mediaType);
    SafeRelease(readerAttributes);
    SafeRelease(reader);
    MFShutdown();
    if (SUCCEEDED(comResult)) CoUninitialize();
  };

  result = MFCreateAttributes(&readerAttributes, 2);
  if (SUCCEEDED(result))
    result = readerAttributes->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING,
                                         TRUE);
  if (SUCCEEDED(result))
    result = readerAttributes->SetUINT32(MF_READWRITE_ENABLE_HARDWARE_TRANSFORMS,
                                         TRUE);
  if (FAILED(result)) {
    shutdown();
    outMessage = "Failed to configure Media Foundation reader attributes.";
    return false;
  }

  result = MFCreateSourceReaderFromURL(std::filesystem::path(localPath).wstring().c_str(),
                                       readerAttributes, &reader);
  if (FAILED(result)) {
    shutdown();
    outMessage = "Failed to open downloaded video for tilemap decode.";
    return false;
  }

  bool formatConfigured = false;
  uint32_t selectedBytesPerPixel = 4;
  struct DecodeSubtype {
    GUID Subtype;
    uint32_t BytesPerPixel;
  };
  const DecodeSubtype preferredSubtypes[] = {
      {MFVideoFormat_RGB32, 4}, {MFVideoFormat_ARGB32, 4}, {MFVideoFormat_RGB24, 3}};
  for (const DecodeSubtype& decodeSubtype : preferredSubtypes) {
    SafeRelease(mediaType);
    result = MFCreateMediaType(&mediaType);
    if (SUCCEEDED(result))
      result = mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if (SUCCEEDED(result))
      result = mediaType->SetGUID(MF_MT_SUBTYPE, decodeSubtype.Subtype);
    if (SUCCEEDED(result))
      result = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                           nullptr, mediaType);
    if (SUCCEEDED(result)) {
      formatConfigured = true;
      selectedBytesPerPixel = decodeSubtype.BytesPerPixel;
      break;
    }
  }

  if (!formatConfigured) {
    shutdown();
    outMessage =
        "Failed to configure a supported RGB decode format (RGB32/ARGB32/RGB24).";
    return false;
  }

  result = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM,
                                       &currentType);
  if (FAILED(result)) {
    shutdown();
    outMessage = "Failed to read configured video media type.";
    return false;
  }

  UINT64 frameSize = 0;
  uint32_t sourceWidth = 0;
  uint32_t sourceHeight = 0;
  UINT64 frameRate = 0;
  float fps = 30.0f;
  if (SUCCEEDED(currentType->GetUINT64(MF_MT_FRAME_SIZE, &frameSize))) {
    sourceWidth = static_cast<uint32_t>(frameSize >> 32);
    sourceHeight = static_cast<uint32_t>(frameSize & 0xFFFFFFFF);
  }
  if (SUCCEEDED(currentType->GetUINT64(MF_MT_FRAME_RATE, &frameRate))) {
    const uint32_t num = static_cast<uint32_t>(frameRate >> 32);
    const uint32_t den = static_cast<uint32_t>(frameRate & 0xFFFFFFFF);
    if (num > 0 && den > 0) fps = static_cast<float>(num) / den;
  }
  if (sourceWidth == 0 || sourceHeight == 0) {
    shutdown();
    outMessage = "Video stream returned invalid frame dimensions.";
    return false;
  }

  std::vector<uint8_t> packedFrames;
  constexpr uint32_t kMaxFrames = 30 * 180;  // 3 minutes cap.
  uint32_t decodedFrames = 0;

  while (decodedFrames < kMaxFrames) {
    DWORD streamFlags = 0;
    IMFSample* sample = nullptr;
    result = reader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, nullptr,
                                &streamFlags, nullptr, &sample);
    if (FAILED(result)) {
      SafeRelease(sample);
      shutdown();
      outMessage = "ReadSample failed while decoding video.";
      return false;
    }

    if (streamFlags & MF_SOURCE_READERF_ENDOFSTREAM) {
      SafeRelease(sample);
      break;
    }

    if (!sample) continue;

    IMFMediaBuffer* buffer = nullptr;
    result = sample->ConvertToContiguousBuffer(&buffer);
    SafeRelease(sample);
    if (FAILED(result)) {
      SafeRelease(buffer);
      shutdown();
      outMessage = "Failed to convert sample buffer while decoding.";
      return false;
    }

    BYTE* data = nullptr;
    DWORD maxLength = 0;
    DWORD currentLength = 0;
    result = buffer->Lock(&data, &maxLength, &currentLength);
    if (FAILED(result)) {
      SafeRelease(buffer);
      shutdown();
      outMessage = "Failed to lock decoded sample buffer.";
      return false;
    }

    const uint32_t stride = sourceWidth * selectedBytesPerPixel;
    if (currentLength < stride * sourceHeight) {
      buffer->Unlock();
      SafeRelease(buffer);
      shutdown();
      outMessage = "Decoded frame buffer size is smaller than expected.";
      return false;
    }

    for (uint32_t y = 0; y < tileHeight; ++y) {
      const uint32_t sourceY = y * sourceHeight / tileHeight;
      const BYTE* sourceRow = data + sourceY * stride;
      for (uint32_t x = 0; x < tileWidth; ++x) {
        const uint32_t sourceX = x * sourceWidth / tileWidth;
        const BYTE* pixel = sourceRow + sourceX * selectedBytesPerPixel;
        const float luminance = 0.114f * pixel[0] + 0.587f * pixel[1] +
                                0.299f * pixel[2];
        packedFrames.push_back(luminance >= 127.0f ? 255 : 0);
      }
    }

    buffer->Unlock();
    SafeRelease(buffer);
    decodedFrames++;
  }

  shutdown();

  if (decodedFrames == 0) {
    outMessage = "No video frames were decoded for tilemap playback.";
    return false;
  }

  outVideo.Width = tileWidth;
  outVideo.Height = tileHeight;
  outVideo.FPS = std::clamp(fps, 1.0f, 60.0f);
  outVideo.Frames = std::move(packedFrames);

  std::ostringstream stream;
  stream << "Decoded " << decodedFrames << " frames at "
         << std::fixed << std::setprecision(2) << outVideo.FPS << " FPS.";
  outMessage = stream.str();
  return true;
#endif
}

bool Play(std::string& outMessage) {
#ifndef _WIN32
  outMessage = "Bad Apple playback is only supported on Windows right now.";
  return false;
#else
  std::string localPath;
  std::string fetchMessage;
  if (!FetchIfNeeded(localPath, fetchMessage)) {
    outMessage = fetchMessage;
    return false;
  }

  const HINSTANCE openResult =
      ShellExecuteW(nullptr, L"open",
                    std::filesystem::path(localPath).wstring().c_str(), nullptr,
                    nullptr, SW_SHOWNORMAL);

  if (reinterpret_cast<intptr_t>(openResult) <= 32) {
    outMessage = "Downloaded video but failed to launch the media player.";
    return false;
  }

  outMessage = "Playing Bad Apple from " + localPath;
  return true;
#endif
}
}  // namespace Mochii::BadApplePlayer
