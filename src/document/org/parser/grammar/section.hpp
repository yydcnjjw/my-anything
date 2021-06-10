#pragma once

#include <org/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct SectionClz;
using section_t = x3::rule<SectionClz, ast::Section>;

BOOST_SPIRIT_DECLARE(section_t);

} // namespace grammar

grammar::section_t const &section();

} // namespace org
} // namespace my
