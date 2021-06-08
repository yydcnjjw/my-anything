#pragma once
#include <org/parser/ast/data_type.hpp>
#include <string>
#include <vector>

namespace my {

namespace org {
namespace ast {

// TODO: 标记元素类型
struct Paragraph : ElementData {
  std::string content;
};

inline std::ostream &operator<<(std::ostream &os, Paragraph const &v) {
  os << " "
     << "Paragraph{\n";
  os << " " << v.content;
  os << " "
     << "}\n";
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Paragraph, content)
