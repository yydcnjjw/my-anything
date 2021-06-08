#pragma once

#include <org/parser/grammar/content.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

content_t const content{"content"};

auto headline_fn = [](auto &ctx) {
  auto &doc_ctx = x3::get<document_ctx_tag>(ctx).get();

  std::cout << "stars: " << x3::_attr(ctx).title << std::endl;
  auto cur_level = x3::_attr(ctx).stars.size();
  std::cout << "level: " << doc_ctx.last_level << " "
            << "cur: " << cur_level  << std::endl;
  x3::_pass(ctx) = doc_ctx.last_level <= cur_level;
  doc_ctx.last_level = cur_level;
};
  
auto const content_def = -org::section() > *(org::headline()[headline_fn]);

BOOST_SPIRIT_DEFINE(content);

struct ContentClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::content_t const &content() { return grammar::content; }

} // namespace org
} // namespace my
