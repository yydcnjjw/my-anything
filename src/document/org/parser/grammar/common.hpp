#pragma once

#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using x3::ascii::blank;
using x3::char_;
using x3::eol;
using x3::eoi;
using x3::lit;
using x3::no_case;
using x3::omit;

using line_t = x3::rule<struct LineClz, std::string>;
BOOST_SPIRIT_DECLARE(line_t);
  
auto constexpr word = x3::alnum | x3::char_('_'); // "\w"
auto constexpr any = x3::char_;                   // "."

decltype(auto) constexpr blank_block(auto &&subject) {
  return x3::omit[*blank] >> subject >> x3::omit[*blank];
}

decltype(auto) constexpr blank_eol_block(auto &&subject) {
  return x3::omit[*blank] >> subject >> x3::omit[*blank] > x3::eol;
}

auto constexpr kleene_blank = x3::omit[*blank];
auto constexpr plus_blank = x3::omit[+blank];
  
} // namespace grammar

grammar::line_t const &line();

} // namespace org
} // namespace my
