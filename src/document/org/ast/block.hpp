#pragma once

#include <org/ast/data_type.hpp>

#include <string>
#include <vector>

namespace my {
namespace org {
namespace ast {

struct Block : GreaterElementData {
  std::string name;
  std::vector<std::string> parameters;
  std::string content;
};

struct GreaterBlock : Block {};

struct DynamicBlock : Block {};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::GreaterBlock, name, parameters, content)
BOOST_FUSION_ADAPT_STRUCT(my::org::ast::DynamicBlock, name, parameters, content)
