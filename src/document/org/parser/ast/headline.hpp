#pragma once

#include <regex>
#include <ranges>

#include <boost/fusion/include/sequence.hpp>

#include <org/parser/ast/content.hpp>

namespace my {
namespace org {

namespace ast {

struct Headline : GreaterElementData {
  std::string stars;
  std::string keyword;
  char priority;
  std::string title;
  std::vector<std::string> tags;

  std::list<Headline> sub_headlines;
};

inline auto extract_tags(std::string const &s) {
  static std::regex const r("[ \t]+(:[[:alnum:]_@#%:]+:)[ \t]*$");
  std::vector<std::string> tags;
  std::smatch m;
  if (std::regex_search(s, m, r)) {
    std::ranges::for_each(std::ranges::subrange(m[1].first + 1, m[1].second) |
                              std::views::split(':'),
                          [&tags](auto const &tag) {
                            auto view = std::string_view(
                                &*tag.begin(), std::ranges::distance(tag));
                            if (!view.empty()) {
                              tags.push_back(std::string(view));
                            }
                          });
  }
  return tags;
}

inline std::ostream &operator<<(std::ostream &os, Headline const &v) {
  os << " "
     << "Headline{\n";
  os << " " << v.stars;
  os << " "
     << " " << v.keyword;
  os << " "
     << " " << v.priority;
  os << " "
     << " " << v.title;

  os << " "
     << " tags: ";
  auto &t = v.tags;
  std::copy(t.begin(), t.end(), std::ostream_iterator<std::string>(os, ":"));

  os << " " << std::endl;
  os << v.content;
  os << " "
     << "}\n";
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

// clang-format off
BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Headline, stars, keyword, priority, title, sub_headlines)
// clang-format off
