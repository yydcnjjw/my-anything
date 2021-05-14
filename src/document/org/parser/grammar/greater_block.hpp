#pragma once

#include <org/parser/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct GreaterBlockClz;
using greater_block_t = x3::rule<GreaterBlockClz, ast::GreaterBlock>;

BOOST_SPIRIT_DECLARE(greater_block_t);

} // namespace grammar

grammar::greater_block_t const &greater_block();
  
} // namespace org
} // namespace my
