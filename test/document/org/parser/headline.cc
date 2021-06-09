#include <gtest/gtest.h>
#include <org/org.hpp>
#include <org/parser/grammar/headline.hpp>

namespace {

auto parse_headline(std::string const &s) {
  return my::org::parse(s, my::org::headline());
}

} // namespace

GTEST_TEST(org_parser_headline, headline) {
  auto _headline = parse_headline("* TODO [#a] 我是:aa :a:b:c::\n");
  ASSERT_TRUE(_headline);
  auto &headline = *_headline;

  ASSERT_EQ(headline.stars, "*");

  ASSERT_TRUE(headline.keyword);
  ASSERT_EQ(*headline.keyword, "TODO");

  ASSERT_TRUE(headline.priority);
  ASSERT_EQ(*headline.priority, 'a');

  ASSERT_EQ(headline.title, "我是:aa");

  ASSERT_EQ(headline.tags, (std::vector<std::string>{"a", "b", "c"}));
}

GTEST_TEST(org_parser_headline, starts) {
  auto _headline = parse_headline("** 我是:aa :a:b:c::\n");
  ASSERT_TRUE(_headline);
  auto &headline = *_headline;

  ASSERT_EQ(headline.stars, "**");

  ASSERT_FALSE(headline.keyword);
  ASSERT_FALSE(headline.priority);

  ASSERT_EQ(headline.title, "我是:aa");
  ASSERT_EQ(headline.tags, (std::vector<std::string>{"a", "b", "c"}));
}

GTEST_TEST(org_parser_headline, optional) {
  auto _headline = parse_headline("* 我是:aa :a:b:c::\n");
  ASSERT_TRUE(_headline);
  auto &headline = *_headline;

  ASSERT_EQ(headline.stars, "*");

  ASSERT_FALSE(headline.keyword);
  ASSERT_FALSE(headline.priority);

  ASSERT_EQ(headline.title, "我是:aa");
  ASSERT_EQ(headline.tags, (std::vector<std::string>{"a", "b", "c"}));
}
