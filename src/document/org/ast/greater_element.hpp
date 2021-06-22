#pragma once

#include <org/ast/block.hpp>
#include <org/ast/drawer.hpp>
#include <org/ast/footnote.hpp>

namespace my {
namespace org {
namespace ast {
  
using greater_element_t =
  x3::variant<GreaterBlock, DynamicBlock, Drawer, FootnoteDef>;

} // namespace ast
} // namespace org
} // namespace my
