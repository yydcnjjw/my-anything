#pragma once

#include <org/ast/data_type.hpp>
#include <org/ast/element.hpp>
#include <org/ast/greater_element.hpp>
#include <org/ast/paragraph.hpp>

namespace my {
namespace org {
namespace ast {

struct Section : GreaterElementData {
  using sub_element_t = flat_variant_t<x3::variant<greater_element_t, element_t>>;

  Section() {}

  Section(std::list<sub_element_t> &&v) : elements{std::move(v)} {}

  Section &operator=(std::list<sub_element_t> &&v) {
    this->elements = std::move(v);
    return *this;
  }

  std::list<sub_element_t> elements;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Section, elements)
