#include <gtest/gtest.h>
#include <org/org.hpp>
#include <org/parser/grammar/block.hpp>

namespace {

auto parse_greater_block(std::string const &s) {
  return my::org::parse(s, my::org::greater_block());
}

auto parse_dynamic_block(std::string const &s) {
  return my::org::parse(s, my::org::dynamic_block());
}  

} // namespace

GTEST_TEST(org_parser_block, greater_block) {
  auto block = parse_greater_block(R"(#+begin_src p1 p2 p3
  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
#+end_src
)");
  ASSERT_TRUE(block);
  ASSERT_EQ(block->name, "src");
  ASSERT_EQ(block->parameters, (std::vector<std::string>{"p1", "p2", "p3"}));
  ASSERT_EQ(block->content, R"(  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
)");
}

GTEST_TEST(org_parser_block, greater_block_whitespace) {
  auto block = parse_greater_block(R"(  #+begin_src p1 p2 p3
  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
  #+end_src   
)");
  ASSERT_TRUE(block);
  ASSERT_EQ(block->name, "src");
  ASSERT_EQ(block->parameters, (std::vector<std::string>{"p1", "p2", "p3"}));
  ASSERT_EQ(block->content, R"(  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
)");
}

GTEST_TEST(org_parser_block, greater_block_name) {
  auto block = parse_greater_block(R"(#+begin_src p1 p2 p3
#+end_src1
)");
  ASSERT_FALSE(block);
}

GTEST_TEST(org_parser_block, greater_block_no_parameters) {
  auto block = parse_greater_block(R"(#+begin_src
#+end_src
)");
  ASSERT_TRUE(block);
  ASSERT_TRUE((*block).parameters.empty());
}

GTEST_TEST(org_parser_block, dynamic_block) {
  auto block = parse_dynamic_block(R"(#+begin: src p1 p2 p3
  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
#+end:
)");
  ASSERT_TRUE(block);
  ASSERT_EQ(block->name, "src");
  ASSERT_EQ(block->parameters, (std::vector<std::string>{"p1", "p2", "p3"}));
  ASSERT_EQ(block->content, R"(  fasfasdfasfasdf
  sdfasf asdf
  asdf
  af
)");
}

GTEST_TEST(org_parser_block, dynamic_block_no_content) {
  auto block = parse_dynamic_block(R"(#+begin: src p1 p2 p3
#+end:
)");
  ASSERT_TRUE(block);
}

GTEST_TEST(org_parser_block, dynamic_block_no_parameters) {
  auto block = parse_dynamic_block(R"(#+begin: src
#+end:
)");
  ASSERT_TRUE(block);
  ASSERT_TRUE((*block).parameters.empty());
}
