#pragma once

#include <string>
#include <vector>

namespace Mochii::BadApplePlayer {
struct TileVideo {
  uint32_t Width = 0;
  uint32_t Height = 0;
  float FPS = 30.0f;
  std::vector<uint8_t> Frames;
};

bool FetchIfNeeded(std::string& outPath, std::string& outMessage);
bool Play(std::string& outMessage);
bool LoadTileVideo(TileVideo& outVideo, std::string& outMessage,
                   uint32_t tileWidth = 64, uint32_t tileHeight = 48);
}  // namespace Mochii::BadApplePlayer
