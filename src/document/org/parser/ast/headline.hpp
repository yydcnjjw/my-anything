#pragma once

#include <org/parser/ast/data_type.hpp>
#include <org/parser/ast/section.hpp>

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

inline std::ostream &operator<<(std::ostream &os, Headline const &v) {
  os << " "
     << "Headline{\n";
  os << " " << v.stars;
  os << " " << v.keyword;
  os << " " << v.priority;
  os << " " << v.title;

  os << " tags: ";
  auto &t = v.tags;
  std::copy(t.begin(), t.end(), std::ostream_iterator<std::string>(os, ":"));

  os << "\n";

  os << v.section;

  os << " " << std::endl;
  std::copy(v.sub_headlines.begin(), v.sub_headlines.end(),
            std::ostream_iterator<Headline>(os, "\n"));
  os << " "
     << "}\n";
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Headline, stars, keyword, priority,
                          title, section, sub_headlines)
