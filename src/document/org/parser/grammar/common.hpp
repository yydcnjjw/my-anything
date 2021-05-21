#pragma once

#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct LineClz;
using line_t = x3::rule<struct LineClz, std::string>;

BOOST_SPIRIT_DECLARE(line_t);

} // namespace grammar

grammar::line_t const &line();

} // namespace org
} // namespace my
