#pragma once

#include <list>
#include <org/parser/ast/headline.hpp>

namespace my {

namespace org {
namespace ast {

struct NodeElement : x3::variant<Headline, Section> {
  using base_type::base_type;
  using base_type::operator=;
};

struct RootElement : x3::position_tagged {
  std::list<NodeElement> elements;
};

using boost::fusion::operator<<;

} // namespace ast

struct Document {
  ast::Content content;
};

inline std::ostream &operator<<(std::ostream &os, Document const &v) {
  os << "Document{\n";
  os << v.content;
  os << "}\n";
  return os;
}
  
} // namespace org

} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::RootElement, elements)
