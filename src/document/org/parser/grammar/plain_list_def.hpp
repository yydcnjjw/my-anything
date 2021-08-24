#pragma once

#include <org/parser/grammar/plain_list.hpp>

namespace my {
namespace org {
namespace grammar {

namespace plain_list {

plain_list_t plain_list{"plain_list"};

struct OrderedBulletClz : x3::annotate_on_success, error_handler_base {};
auto const ordered_bullet = x3::rule<OrderedBulletClz, std::string>{
    "ordered_bullet"} = x3::uint_ > x3::char_(".)");
struct UnorderedBulletClz : x3::annotate_on_success, error_handler_base {};
auto const unordered_bullet =
    x3::rule<UnorderedBulletClz, std::string>{"unordered_bullet"} =
        x3::char_("-+*");

struct BulletClz : x3::annotate_on_success, error_handler_base {};
auto const bullet = x3::rule<BulletClz, std::string>{"bullet"} =
    (ordered_bullet | unordered_bullet) > ' ';

struct CounterSetClz : x3::annotate_on_success, error_handler_base {};
auto const counter_set = x3::rule<CounterSetClz, uint>{"counter_set"} =
    "[@" > x3::uint_ > ']';

struct CheckboxClz : x3::annotate_on_success, error_handler_base {};
auto const checkbox = x3::rule<CheckboxClz, char>{
    "checkbox"} = "[" > x3::char_(" -X") > "]";

struct TagTextClz : x3::annotate_on_success, error_handler_base {};
auto const tag_text = x3::rule<TagTextClz, std::string>{"tag_text"} =
    +(x3::char_ - x3::eol);
auto const tag_separator = "::";
struct TagClz : x3::annotate_on_success, error_handler_base {};
auto const tag = x3::rule<TagClz, std::string>{"tag"} =
    tag_text > ' ' > tag_separator;

  // auto const content = x3::;

struct OrderedItemClz : x3::annotate_on_success, error_handler_base {};
auto const ordered_item =
    x3::rule<OrderedItemClz, ast::PlainListItem>{"ordered_item"} =
        ordered_bullet >> -counter_set >> -checkbox >> content;

struct UnOrderedItemClz : x3::annotate_on_success, error_handler_base {};
auto const unordered_item =
    x3::rule<UnOrderedItemClz, ast::PlainListItem>{"unordered_item"} =
        ordered_bullet >> -counter_set >> -checkbox >> -tag >> content;

auto const plain_list_def = +(ordered_item | unordered_item);

} // namespace plain_list

struct PlainListClz : x3::annotate_on_success, error_handler_base {};

BOOST_SPIRIT_DEFINE(plain_list::plain_list)

} // namespace grammar

grammar::plain_list_t const &plain_list() {
  return grammar::plain_list::plain_list;
}

} // namespace org
} // namespace my
