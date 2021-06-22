#pragma once

#include <org/ast/block.hpp>
#include <org/ast/drawer.hpp>
#include <org/ast/footnote.hpp>
#include <org/ast/paragraph.hpp>

namespace my {
namespace org {
namespace ast {

using element_t =
  x3::variant<Paragraph>;
  
using greater_element_t =
  x3::variant<GreaterBlock, DynamicBlock, Drawer, FootnoteDef>;

} // namespace ast
} // namespace org
} // namespace my
