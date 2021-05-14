#pragma once

#include <org/parser/ast/data_type.hpp>
#include <org/parser/ast/greater_block.hpp>

namespace my {

namespace org {
namespace ast {

struct Section : GreaterElementData {
  std::string content;
};

inline std::ostream &operator<<(std::ostream &os, Section const &v) {
  os << "Section{\n";
  os << v.content;
  os << "}\n";
  return os;
}
  
} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Section, content)
