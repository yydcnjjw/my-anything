#pragma once

#include <org/parser/grammar/document.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::eoi;
using x3::eps;

document_t const document{"document"};

x3::rule<struct TopElementClz, ast::NodeElement> const top_element{
    "top_element"};
auto const top_element_def = headline() | section();
auto const document_def = eps > +top_element > eoi;

BOOST_SPIRIT_DEFINE(document, top_element);

struct DocumentClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::document_t const &document() { return grammar::document; }

} // namespace org
} // namespace my
