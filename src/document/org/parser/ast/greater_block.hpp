#pragma once
#include <org/parser/ast/data_type.hpp>
#include <string>
#include <vector>

namespace my {

namespace org {
namespace ast {

struct GreaterBlock : GreaterElementData {
  std::string name;
  std::vector<std::string> parameters;
  std::string content;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::GreaterBlock, name, parameters, content)
