#pragma once

#include <org/ast/data_type.hpp>
#include <org/ast/element.hpp>

namespace my {
namespace org {
namespace ast {
  
struct FootnoteDef : GreaterElementData {
  std::string label;
  std::list<element_t> elements;

  FootnoteDef() {}

  FootnoteDef(std::list<element_t> &&v) : elements{std::move(v)} {}

  FootnoteDef &operator=(std::list<element_t> &&v) {
    this->elements = std::move(v);
    return *this;
  }
  
};
  
}
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::FootnoteDef, label, elements)
