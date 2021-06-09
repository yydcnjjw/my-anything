#pragma once

#include <org/parser/grammar/document.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::eoi;
using x3::eps;

auto document_op = [](auto &ctx) {
  auto &headline = x3::_attr(ctx);
  std::cout << "doc: " << headline.title << std::endl;
};

document_t const document{"document"};
auto const document_def = eps > -org::section() >
                          *(org::headline() / document_op) > eoi;

BOOST_SPIRIT_DEFINE(document);

struct DocumentClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::document_t const &document() { return grammar::document; }

} // namespace org
} // namespace my
