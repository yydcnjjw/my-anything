#pragma once

#include <org/ast/data_type.hpp>

namespace my {
namespace org {
namespace ast {

struct PlainListItem {
  std::string bullet;
  optional<char> checkbox;
  optional<std::size_t> counter;
  optional<std::string> tag;
  std::string content;
};

struct PlainList : GreaterElementData {
  using sub_element_t = x3::variant<PlainListItem, PlainList>;

  std::list<sub_element_t> elements;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::PlainList, elements)
BOOST_FUSION_ADAPT_STRUCT(my::org::ast::PlainListItem, bullet, checkbox,
                          counter, tag, content)
