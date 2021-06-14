#pragma once

#include <org/parser/grammar/common.hpp>

namespace my {
namespace org {
namespace grammar {

using x3::char_;
using x3::eol;

line_t line{"line"};
auto const line_def{*(any - eol) > -char_('\r') > char_('\n')};
struct LineClz : x3::annotate_on_success, error_handler_base {};

BOOST_SPIRIT_DEFINE(line);

} // namespace grammar

grammar::line_t const &line() { return grammar::line; }

} // namespace org
} // namespace my
