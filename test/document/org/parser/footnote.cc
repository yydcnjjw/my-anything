#include <gtest/gtest.h>
#include <org/org.hpp>
#include <org/parser/grammar/footnote.hpp>

using namespace my;

namespace {

auto parse_footnote_def(std::string const &s) {
  return my::org::parse(s, org::footnote_def());
}

} // namespace

GTEST_TEST(org_parser_footnote, footnote_def) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]afasfasf)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  auto it = (*footnote_def).elements.begin();
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(afasfasf)");
}

GTEST_TEST(org_parser_footnote, footnote_def_empty_content) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B])");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");
  ASSERT_TRUE((*footnote_def).elements.empty());
}

GTEST_TEST(org_parser_footnote, footnote_def_multi_line) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");

  auto it = (*footnote_def).elements.begin();
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(fasfaf
)");
  ++it;
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(asfasf)");
}

GTEST_TEST(org_parser_footnote, footnote_def_headline) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf
* headline)");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");

  auto it = (*footnote_def).elements.begin();
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(fasfaf
)");
  ++it;
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(asfasf
)");
}

GTEST_TEST(org_parser_footnote, footnote_def_multifootnote) {
  auto footnote_def = parse_footnote_def(R"([fn:1_a-B]fasfaf
asfasf
[fn:1_a-B])");

  ASSERT_TRUE(footnote_def);
  ASSERT_EQ((*footnote_def).label, "1_a-B");

  auto it = (*footnote_def).elements.begin();
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(fasfaf
)");
  ++it;
  ASSERT_EQ(boost::get<org::ast::Paragraph>(*it).content, R"(asfasf
)");
}
