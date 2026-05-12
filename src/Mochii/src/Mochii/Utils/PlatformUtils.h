#pragma once
#include <optional>
#include <string>

namespace Mochii {
class FileDialogs {
 public:
  static std::optional<std::string> OpenFile(const char* filter);
  static std::optional<std::string> SaveFile(const char* filter);
};
}  // namespace Mochii
