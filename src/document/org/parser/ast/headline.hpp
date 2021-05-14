#pragma once

#include <boost/fusion/adapted/adt/adapt_adt.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

#include <regex>

#include <org/parser/ast/data_type.hpp>
#include <org/parser/ast/section.hpp>

namespace my {
namespace org {

namespace ast {

class Headline;

struct Content {
  std::optional<Section *> section;
  std::list<Headline *> headlines;
};

class Headline : public GreaterElementData {
public:
  std::string const &stars() const { return this->_stars; }
  void stars(std::string const &stars) { this->_stars = stars; }

  std::optional<std::string> const &keyword() const { return this->_keyword; }
  void keyword(std::string const &keyword) { this->_keyword = keyword; }

  std::optional<char> const &priority() const { return this->_priority; }
  void priority(char v) { this->_priority = v; }

  std::string const &title() const { return this->_title; }
  void title(std::string const &title) {
    this->_title = title.substr(0, this->extract_tags(title));
  }

  std::vector<std::string> const &tags() const { return this->_tags; }

  void tags(std::string const &tags) { this->extract_tags(tags); }

  void tags(std::vector<std::string> const &tags) { this->_tags = tags; }

  Content content;

private:
  std::string _stars;
  std::optional<std::string> _keyword;
  std::optional<char> _priority;
  std::string _title;
  std::vector<std::string> _tags;

  std::smatch::difference_type extract_tags(std::string const &s) {
    static std::regex const r("[ \t]+(:[[:alnum:]_@#%:]+:)[ \t]*$");
    std::smatch m;
    if (std::regex_search(s, m, r)) {
      this->_tags.clear();
      std::ranges::for_each(std::ranges::subrange(m[1].first + 1, m[1].second) |
                                std::views::split(':'),
                            [this](auto const &tag) {
                              std::string s{std::string_view(
                                  &*tag.begin(), std::ranges::distance(tag))};
                              if (!s.empty()) {
                                this->_tags.push_back(std::move(s));
                              }
                            });
    }
    return m.position();
  }
};

inline std::ostream &operator<<(std::ostream &os, Content const &v);

inline std::ostream &operator<<(std::ostream &os, Headline const &v) {
  os << "Headline{\n";
  os << v.stars();
  if (v.keyword()) {
    os << " " << *v.keyword();
  }
  if (v.priority()) {
    os << " " << *v.priority();
  }
  os << " " << v.title();

  os << " ";
  auto &t = v.tags();
  std::copy(t.begin(), t.end(), std::ostream_iterator<std::string>(os, ":"));

  os << std::endl;
  os << v.content;
  os << "}\n";
  return os;
}

std::ostream &operator<<(std::ostream &os, Content const &v) {
  if (v.section) {
    os << **v.section;
  }

  for (auto &headline : v.headlines) {
    os << *headline;
  }
  return os;
}

} // namespace ast
} // namespace org
} // namespace my

// clang-format off
BOOST_FUSION_ADAPT_ADT(my::org::ast::Headline,
                       (obj.stars(), obj.stars(val))
                       (obj.keyword(), obj.keyword(val))
                       (obj.priority(), obj.priority(val))
                       (obj.title(), obj.title(val)))
// clang-format off
