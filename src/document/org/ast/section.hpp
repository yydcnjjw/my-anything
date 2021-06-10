#pragma once

#include <org/ast/data_type.hpp>
#include <org/ast/greater_block.hpp>
#include <org/ast/paragraph.hpp>

namespace my {
namespace org {
namespace ast {

struct Section : GreaterElementData {
  struct SubElement : x3::variant<GreaterBlock, Paragraph> {
    using base_type::base_type;
    using base_type::operator=;
  };

  Section() {
  }

  Section(std::list<SubElement> &&v) : elements{std::move(v)} {}

  Section& operator=(std::list<SubElement> &&v) {
    this->elements = std::move(v);
    return *this;
  }

  std::list<SubElement> elements;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Section, elements)
