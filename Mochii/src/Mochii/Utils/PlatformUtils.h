#pragma once
#include <string>

namespace Mochii {
class FileDialogs {
 public:
  static std::string OpenFile(const char* filter);
  static std::string SaveFile(const char* filter);
};
}  // namespace Mochii
