#pragma once

#include <org/ast/paragraph.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using paragraph_t = x3::rule<struct ParagraphClz, ast::Paragraph>;

BOOST_SPIRIT_DECLARE(paragraph_t);

} // namespace grammar

grammar::paragraph_t const &paragraph();
  
} // namespace org
} // namespace my
