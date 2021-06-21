#pragma once

#include <org/ast/data_type.hpp>

namespace my {
namespace org {
namespace ast {

struct FootnoteDef : GreaterElementData {
  std::string label;
  std::string content;
};
  
}
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::FootnoteDef, label, content)
