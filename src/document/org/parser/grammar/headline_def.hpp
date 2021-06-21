#pragma once

#include <boost/regex/icu.hpp>
#include <ranges>

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

inline auto extract_tags(std::string const &s) {
  static auto const r =
      boost::make_u32regex("[ \t]+(:[[:alnum:]_@#%:]+:)[ \t]*$");
  std::vector<std::string> tags;
  boost::smatch m;
  boost::smatch::difference_type pos{0};
  if (boost::u32regex_search(s, m, r)) {
    std::ranges::for_each(std::ranges::subrange(m[1].first + 1, m[1].second) |
                              std::views::split(':'),
                          [&tags](auto const &tag) {
                            auto view = std::string_view(
                                &*tag.begin(), std::ranges::distance(tag));
                            if (!view.empty()) {
                              tags.push_back(std::string(view));
                            }
                          });
    pos = m.position();
  } else {
    pos = s.size();
  }

  return std::make_pair(tags, pos);
}

namespace headline {

using x3::lexeme;
using x3::omit;
using x3::raw;

auto stars_op = [](auto &ctx) {
  auto curlevel = x3::_attr(ctx).size();
  auto &doc_ctx = x3::get<document_ctx_tag>(ctx).get();
  auto lastlevel = doc_ctx.levels.top();
  if (curlevel > lastlevel) {
    x3::_pass(ctx) = true;
    doc_ctx.levels.push(curlevel);
  } else {
    x3::_pass(ctx) = false;
    doc_ctx.levels.pop();
  }
};

struct StarsClz : x3::annotate_on_success, error_handler_base {};
auto const stars = x3::rule<StarsClz, std::string, true>{"stars"} =
    (+char_('*'))[stars_op];

struct KeywordClz : x3::annotate_on_success, error_handler_base {};
auto const keyword = x3::rule<struct KeywordClz, std::string>{"keyword"} =
    +char_("A-Z");

struct PriorityClz : x3::annotate_on_success, error_handler_base {};
auto const priority = x3::rule<PriorityClz, char>{
    "priority"} = "[#" > char_("a-zA-Z") > ']';

struct TagsClz : x3::annotate_on_success, error_handler_base {};
auto const tags = x3::rule<TagsClz, std::vector<std::string>>{"tags"} =
    x3::attr(std::vector<std::string>{});

auto tags_op = [](auto &ctx) {
  auto &headline = x3::_val(ctx);
  auto &title = headline.title;
  auto &tags = x3::_attr(ctx);

  auto result = extract_tags(title);
  title.erase(result.second);
  tags = std::move(result.first);
};

struct TitleClz : x3::annotate_on_success, error_handler_base {};
auto const title = x3::rule<TitleClz, std::string>{"title"} = (+(any - eol));

headline_t const headline{"headline"};
auto const headline_def{stars > plus_blank > -(keyword > plus_blank) >
                        -(priority > kleene_blank) > -title > tags[tags_op] >
                        eol > -org::section() > *headline};

} // namespace headline

BOOST_SPIRIT_DEFINE(headline::headline);

struct HeadlineClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::headline_t const &headline() { return grammar::headline::headline; }

} // namespace org
} // namespace my
