#include <gtest/gtest.h>
#include <org/org.hpp>
#include <fstream>
#include <sstream>

using my::org::Document;
using my::org::element_visitor;
using my::org::parse_document;

GTEST_TEST(org_parser_document, document_string) {

  std::ifstream ifs{"document_test.org"};
  
  std::stringstream oss;

  oss << ifs.rdbuf();
  
  auto s = oss.str();

  auto ast = parse_document(s);

  Document doc;
  element_visitor visitor{doc};

  for (auto &element : ast->elements) {
    element.apply_visitor(visitor);
  }

  ASSERT_TRUE(ast);

  std::cout << doc << std::endl;
}

GTEST_TEST(org_parser_document, document_stream) {

  std::ifstream ifs{"document_test.org"};
  auto ast = parse_document(ifs);

  Document doc;
  element_visitor visitor{doc};

  for (auto &element : ast->elements) {
    element.apply_visitor(visitor);
  }

  ASSERT_TRUE(ast);

  std::cout << doc << std::endl;
}
