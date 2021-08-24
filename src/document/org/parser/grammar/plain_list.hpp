#pragma once
#include <org/ast/plain_list.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using plain_list_t = x3::rule<struct PlainListClz, ast::PlainList>;

BOOST_SPIRIT_DECLARE(plain_list_t);

} // namespace grammar

grammar::plain_list_t const &plain_list();

} // namespace org
} // namespace my
