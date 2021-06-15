#include <gtest/gtest.h>
#include <org/org.hpp>
#include <org/parser/grammar/drawer.hpp>

namespace {

auto parse_drawer(std::string const &s) {
  return my::org::parse(s, my::org::drawer());
}

} // namespace

GTEST_TEST(org_parser_drawer, drawer) {
  auto drawer = parse_drawer(R"(:te-_st:
fasdfsf
fadsfsa
df
:end:
)");

  ASSERT_TRUE(drawer);
  ASSERT_EQ((*drawer).name, "te-_st");
  ASSERT_EQ((*drawer).content, R"(fasdfsf
fadsfsa
df
)");
}

GTEST_TEST(org_parser_drawer, drawer_blank) {
  auto drawer = parse_drawer(R"(  :te-_st:  
fasdfsf
fadsfsa
df
  :end: 
)");

  ASSERT_TRUE(drawer);
  ASSERT_EQ((*drawer).name, "te-_st");
  ASSERT_EQ((*drawer).content, R"(fasdfsf
fadsfsa
df
)");
}

// GTEST_TEST(org_parser_drawer, drawer_unicode) {
//   auto drawer = parse_drawer(R"(:我是:
// fasdfsf
// fadsfsa
// df
// :end:)");

//   ASSERT_TRUE(drawer);
//   ASSERT_EQ((*drawer).name, "我是");
//   ASSERT_EQ((*drawer).content, R"(fasdfsf
// fadsfsa
// df
// )");
// }
