#pragma once

#include <org/ast/drawer.hpp>
#include <org/parser/config.hpp>

namespace my {

namespace org {
namespace grammar {

using drawer_t = x3::rule<struct DrawerClz, ast::Drawer>;

BOOST_SPIRIT_DECLARE(drawer_t);

} // namespace grammar

grammar::drawer_t const &drawer();

} // namespace org

} // namespace my
