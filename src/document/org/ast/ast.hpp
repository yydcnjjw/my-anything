#pragma once

#include <org/ast/types.hpp>
#include <org/ast/section.hpp>
#include <org/ast/headline.hpp>

namespace my {

namespace org {
namespace ast {

struct Document : x3::position_tagged {
  optional<Section> section;
  std::list<Headline> headlines;
};
  
inline std::ostream &operator<<(std::ostream &os, Document const &v) {
  os << "Document{\n";
  os << v.section;

  for (auto &headline : v.headlines) {
    os << headline;
  }
  os << "}\n";
  return os;
}

} // namespace ast
  
} // namespace org

} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Document, section, headlines)
