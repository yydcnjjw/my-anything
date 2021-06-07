#pragma once

#include <org/parser/grammar/greater_block.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/paragraph.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::eoi;
using x3::eol;

namespace section {

x3::rule<struct SectionSubElementClz, ast::Section::SubElement> const
    section_sub_element{"section_sub_element"};
auto const section_sub_element_def = org::greater_block() | org::paragraph();

BOOST_SPIRIT_DEFINE(section_sub_element);

section_t const section{"section"};
auto const section_def{+(section_sub_element - org::headline())};

} // namespace section

BOOST_SPIRIT_DEFINE(section::section);

struct SectionClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::section_t const &section() { return grammar::section::section; }

} // namespace org
} // namespace my
