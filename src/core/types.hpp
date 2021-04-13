#pragma once

#include <any>
#include <filesystem>
#include <map>
#include <set>

#include <boost/format.hpp>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

#include <rx.hpp>

namespace my {
namespace fs = std::filesystem;
namespace rx = rxcpp;

} // namespace my
