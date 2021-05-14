#include <gtest/gtest.h>
#include <org/org.hpp>

using my::org::Document;
using my::org::element_visitor;
using my::org::parse_document;

GTEST_TEST(org_parser_document, document) {

  std::string s{R"(
fdaffd

fasdfsf
aaafff
ddd
* headline1
section2
** headline11
section3

* headline2
** headline21
)"};
  auto ast = parse_document(s);

  Document doc;
  element_visitor visitor{doc};

  for (auto &element : ast->elements) {
    element.apply_visitor(visitor);
  }

  ASSERT_TRUE(ast);

  std::cout << doc << std::endl;
}
