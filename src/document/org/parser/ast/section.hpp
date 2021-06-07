#pragma once

#include <org/parser/ast/data_type.hpp>
#include <org/parser/ast/greater_block.hpp>
#include <org/parser/ast/paragraph.hpp>

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

inline std::ostream &operator<<(std::ostream &os, Section const &v) {
  os << "Section{\n";
  for (auto &e : v.elements) {
    boost::apply_visitor([&](auto &v) { os << v; }, e);
  }
  os << "}\n";
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Section, elements)
