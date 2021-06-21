#pragma once

#include <org/ast/block.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using greater_block_t = x3::rule<struct GreaterBlockClz, ast::GreaterBlock, true>;

using dynamic_block_t = x3::rule<struct DynamicBlockClz, ast::DynamicBlock, true>;

BOOST_SPIRIT_DECLARE(greater_block_t, dynamic_block_t);

} // namespace grammar

grammar::greater_block_t const &greater_block();
grammar::dynamic_block_t const &dynamic_block();

} // namespace org
} // namespace my
