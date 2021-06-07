#pragma once

#include <org/parser/ast/content.hpp>
#include <org/parser/ast/headline.hpp>

namespace my {
namespace org {
namespace ast {

std::ostream &operator<<(std::ostream &os, Content const &v) {
  if (v.section) {
    os << v.section.value();
  }

  for (auto &headline : v.headlines) {
    os << headline;
  }
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Content, section, headlines)
