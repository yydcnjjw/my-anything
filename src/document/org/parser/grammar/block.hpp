#pragma once

#include <org/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct GreaterBlockClz;
using greater_block_t = x3::rule<GreaterBlockClz, ast::GreaterBlock, true>;

struct DynamicBlockClz;
using dynamic_block_t = x3::rule<DynamicBlockClz, ast::DynamicBlock, true>;

BOOST_SPIRIT_DECLARE(greater_block_t, dynamic_block_t);

} // namespace grammar

grammar::greater_block_t const &greater_block();
grammar::dynamic_block_t const &dynamic_block();

} // namespace org
} // namespace my
