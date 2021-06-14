#pragma once

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/block.hpp>
#include <org/parser/grammar/drawer.hpp>
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

auto const headline_header = (+char_('*') > ' ' > *(any - eol) > eol);

x3::rule<struct SectionSubElementClz, ast::Section::SubElement> const
    section_sub_element{"section_sub_element"};
auto const section_sub_element_def = org::greater_block() |
                                     org::dynamic_block() | org::drawer() |
                                     org::paragraph();

BOOST_SPIRIT_DEFINE(section_sub_element);
section_t const section{"section"};
auto const section_def{+(section_sub_element - headline_header)};

} // namespace section

BOOST_SPIRIT_DEFINE(section::section);

struct SectionClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::section_t const &section() { return grammar::section::section; }

} // namespace org
} // namespace my
