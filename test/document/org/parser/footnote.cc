#include <gtest/gtest.h>
#include <org/org.hpp>
#include <org/parser/grammar/footnote.hpp>

namespace {

auto parse_footnote_def(std::string const &s) {
  return my::org::parse(s, my::org::footnote_def());
}

} // namespace

GTEST_TEST(org_parser_footnote, footnote_def) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]afasfasf)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_EQ((*footnote_def).content, R"(afasfasf)");
}

GTEST_TEST(org_parser_footnote, footnote_def_empty_content) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B])");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_EQ((*footnote_def).content, R"()");
}

GTEST_TEST(org_parser_footnote, footnote_def_multi_line) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf
afasfd
asdfafafafa
f
af)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_EQ((*footnote_def).content, R"(fasfaf
asfasf
afasfd
asdfafafafa
f
af)");
}

GTEST_TEST(org_parser_footnote, footnote_def_headline) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf
* headline)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_EQ((*footnote_def).content, R"(fasfaf
asfasf
)");
}

GTEST_TEST(org_parser_footnote, footnote_def_multifootnote) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf
* headline)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_EQ((*footnote_def).content, R"(fasfaf
asfasf
)");
}
