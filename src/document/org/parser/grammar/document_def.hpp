#pragma once

#include <org/parser/grammar/document.hpp>
#include <org/parser/grammar/content.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::eoi;
using x3::eps;

document_t const document{"document"};

auto const document_def = eps > org::content() > eoi;

BOOST_SPIRIT_DEFINE(document);

struct DocumentClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::document_t const &document() { return grammar::document; }

} // namespace org
} // namespace my
