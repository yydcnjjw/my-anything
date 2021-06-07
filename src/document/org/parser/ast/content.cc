#include "content.hpp"

#include <org/parser/ast/headline.hpp>
#include <org/parser/ast/section.hpp>

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
