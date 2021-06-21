#pragma once

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/drawer.hpp>

namespace my {
namespace org {
namespace grammar {
namespace drawer {

auto const name = +(word | char_('-'));
auto const begin = blank_eol_block(':' > name > ':');
auto const end = blank_eol_block(no_case[":end:"]);
auto const content = *(org::line() - end);

drawer_t const drawer{"drawer"};
auto const drawer_def = begin > content > end;

} // namespace drawer

BOOST_SPIRIT_DEFINE(drawer::drawer);

struct DrawerClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::drawer_t const &drawer() { return grammar::drawer::drawer; }

} // namespace org
} // namespace my
