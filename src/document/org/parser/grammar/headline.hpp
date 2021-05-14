#pragma once

#include <org/parser/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {

namespace org {
namespace grammar {

struct HeadlineClz;
using headline_t = x3::rule<HeadlineClz, ast::Headline>;

BOOST_SPIRIT_DECLARE(headline_t);

} // namespace grammar

grammar::headline_t const &headline();

} // namespace org

} // namespace my
