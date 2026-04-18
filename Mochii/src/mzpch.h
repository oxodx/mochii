#pragma once

// GLAD must be included first before any other GL headers
#include <glad/glad.h>

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "Mochii/Core/Log.h"
#include "Mochii/Debug/instrumentor.h"

#ifdef MI_PLATFORM_WINDOWS
#include <Windows.h>
#endif
