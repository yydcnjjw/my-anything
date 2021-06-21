#pragma once

#include <org/ast/section.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using section_t = x3::rule<struct SectionClz, ast::Section>;

BOOST_SPIRIT_DECLARE(section_t);

} // namespace grammar

grammar::section_t const &section();

} // namespace org
} // namespace my
