#pragma once


#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/element.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/paragraph.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {
namespace grammar {
namespace section {

using x3::char_;
using x3::eol;

x3::rule<struct SectionSubElementClz, ast::Section::sub_element_t> const
    section_sub_element{"section_sub_element"};
auto const section_sub_element_def = greater_element | element;

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
