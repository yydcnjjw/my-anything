#pragma once

#include <org/parser/grammar/common.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::eol;

line_t line{"line"};
auto const line_def{*(char_ - eol) > -char_('\r') > char_('\n')};

BOOST_SPIRIT_DEFINE(line);

} // namespace grammar

grammar::line_t const &line() { return grammar::line; }

} // namespace org
} // namespace my
