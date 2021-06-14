#pragma once

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/drawer.hpp>

namespace my {
namespace org {
namespace grammar {

using x3::char_;
using x3::eol;
using x3::lit;
using x3::no_case;

namespace drawer {

auto const name = +(word | x3::char_('-'));
auto const begin = ':' > name > ':' > eol;
auto const end = no_case[":end:"];
auto const content = *(org::line() - end);

drawer_t const drawer{"drawer"};
auto const drawer_def = begin > content > end;

} // namespace drawer

BOOST_SPIRIT_DEFINE(drawer::drawer);

struct DrawerClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::drawer_t const &drawer() {
  return grammar::drawer::drawer;
}
 
} // namespace org
} // namespace my
