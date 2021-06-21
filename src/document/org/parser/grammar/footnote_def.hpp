#pragma once

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/footnote.hpp>

namespace my {
namespace org {
namespace grammar {
namespace footnote {

auto const label = x3::rule<struct _, std::string>{"label"} =
    +(word | char_('-'));
auto const footnote_def_head =
    x3::rule<struct _, std::string>{"footnote_def_head"} = "[fn:" > label > "]";
auto const content = x3::rule<struct _, std::string>{"content"} =
    *(org::line() - footnote_def_head);

footnote_def_t const footnote_def{"footnote_def"};
auto const footnote_def_def = footnote_def_head >> content;

} // namespace footnote

BOOST_SPIRIT_DEFINE(footnote::footnote_def);

struct FootnoteDefClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::footnote_def_t const &footnote_def() {
  return grammar::footnote::footnote_def;
}

} // namespace org
} // namespace my
