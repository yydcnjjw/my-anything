#pragma once

#include <org/ast/data_type.hpp>

namespace my {
namespace org {
namespace ast {

struct Drawer : GreaterElementData {
  std::string name;
  std::string content;
};
  
}
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Drawer, name, content)
