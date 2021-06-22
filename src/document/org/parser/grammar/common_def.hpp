#pragma once

#include <org/parser/grammar/common.hpp>

namespace my {
namespace org {
namespace grammar {

line_t line{"line"};
auto const crlf = (-char_('\r') >> char_('\n'));
// clang-format off
auto const line_def = (+(any - eol) >> (crlf | eoi)) | crlf;
// clang-format off
struct LineClz : x3::annotate_on_success, error_handler_base {};

BOOST_SPIRIT_DEFINE(line);

} // namespace grammar

grammar::line_t const &line() { return grammar::line; }

} // namespace org
} // namespace my
