#pragma once

#include <org/ast/data_type.hpp>

#include <string>
#include <vector>

namespace my {
namespace org {
namespace ast {

struct Paragraph : ElementData {
  std::string content;
};

} // namespace ast
} // namespace org
} // namespace my

BOOST_FUSION_ADAPT_STRUCT(my::org::ast::Paragraph, content)
