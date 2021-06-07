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
  
} // namespace ast
  
} // namespace org

} // namespace my
