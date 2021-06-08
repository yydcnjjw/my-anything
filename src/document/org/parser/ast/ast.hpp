#pragma once

#include <org/parser/ast/content.hpp>

namespace my {

namespace org {
namespace ast {

struct Document : Content {
  Document &operator=(Content &&v) {
    Content::operator=(std::move(v));
    return *this;
  }
};

using boost::fusion::operator<<;

inline std::ostream &operator<<(std::ostream &os, Document const &v) {
  os << "Document{\n";
  os << reinterpret_cast<Content const&>(v);
  os << "}\n";
  return os;
}
  
} // namespace ast
  
} // namespace org

} // namespace my
