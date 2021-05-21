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

inline std::ostream &operator<<(std::ostream &os, GreaterBlock const &v) {
  os << "GreaterBlock{\n";
  os << v.name << std::endl;
  for (auto &p : v.parameters) {
    os << p << ' ';
  }
  os << std::endl;
  os << v.content;
  os << "}\n";
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::GreaterBlock, name, parameters, content)
