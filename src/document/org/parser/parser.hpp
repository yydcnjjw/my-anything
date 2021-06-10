#pragma once

#include <org/ast/ast.hpp>
#include <org/parser/config.hpp>
#include <org/parser/error_handler.hpp>
#include <org/parser/grammar/document.hpp>

namespace my {
namespace org {

namespace detail {
template <typename Iterator, typename Parser>
optional<typename Parser::attribute_type>
parse(Iterator &begin, Iterator end, Parser const &parser) {
  using attribute_type = typename Parser::attribute_type;
  attribute_type v;

  error_handler<Iterator> err_handler(begin, end, std::cerr);
  grammar::DocumentCtx<Iterator> ctx;

  if (x3::parse(
          begin, end,
          x3::with<grammar::document_ctx_tag>(std::ref(
              ctx))[x3::with<error_handler_tag>(std::ref(err_handler))[parser]],
          v)) {
    return v;
  } else {
    return NULL_OPT;
  }
}
} // namespace detail

template <typename Parser>
auto parse(std::string const &s, Parser const &parser) {
  auto begin{s.begin()}, end{s.end()};
  return detail::parse(begin, end, parser);
}

template <typename Parser>
auto parse(std::istream &is, Parser const &parser) {
  stream_iterator_type begin{
      boost::spirit::make_default_multi_pass(streambuf_iterator_type{is})},
      end{};
  return detail::parse(begin, end, parser);
}

template <typename T> auto parse_document(T &t) {
  return parse(t, org::document());
}

} // namespace org

} // namespace my
