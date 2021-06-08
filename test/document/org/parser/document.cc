#include <gtest/gtest.h>
#include <org/org.hpp>
#include <fstream>
#include <sstream>

using my::org::ast::Document;
using my::org::parse_document;

// GTEST_TEST(org_parser_document, document_string) {

//   std::ifstream ifs{"document_test.org"};

//   std::stringstream oss;

//   oss << ifs.rdbuf();

//   auto s = oss.str();

//   auto ast = parse_document(s);

//   ASSERT_TRUE(ast);

//   std::cout << ast.value() << std::endl;
// }

GTEST_TEST(org_parser_document, document_stream) {

  std::ifstream ifs{"document_test.org"};
  auto ast = parse_document(ifs);

  ASSERT_TRUE(ast);

  std::cout << ast.value() << std::endl;
}
