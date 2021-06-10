#pragma once

#include <org/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct ParagraphClz;
using paragraph_t = x3::rule<ParagraphClz, ast::Paragraph>;

BOOST_SPIRIT_DECLARE(paragraph_t);

} // namespace grammar

grammar::paragraph_t const &paragraph();
  
} // namespace org
} // namespace my
