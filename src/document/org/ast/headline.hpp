#pragma once

#include <org/ast/data_type.hpp>
#include <org/ast/section.hpp>

namespace my {
namespace org {
namespace ast {

struct Headline : GreaterElementData {
  std::string stars;
  optional<std::string> keyword;
  optional<char> priority;
  std::string title;
  std::vector<std::string> tags;

  optional<Section> section;
  std::list<Headline> sub_headlines;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Headline, stars, keyword, priority,
                          title, tags, section, sub_headlines)
