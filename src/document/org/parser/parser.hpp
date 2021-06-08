#pragma once

#include <org/parser/ast/ast.hpp>
#include <org/parser/config.hpp>
#include <org/parser/error_handler.hpp>
#include <org/parser/grammar/document.hpp>

namespace my {
namespace org {

namespace detail {
template <typename Iterator, typename Parser>
std::optional<typename Parser::attribute_type>
parse(Iterator &begin, Iterator end, Parser const &parser) {
  static_assert(x3::traits::is_rule<Parser>::value, "Parser must is a rule");
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
    return std::nullopt;
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

// struct element_visitor : public boost::static_visitor<> {

//   Document &doc;
//   std::stack<ast::Headline *> headline_ctx;

//   element_visitor(Document &doc) : doc(doc) {}

//   void operator()(ast::Headline &e) {
//     if (headline_ctx.empty()) {
//       headline_ctx.push(&e);
//       doc.content.headlines.push_back(&e);
//     } else {
//       auto cur = headline_ctx.top();
//       if (cur->stars().length() < e.stars().length()) {
//         headline_ctx.push(&e);
//         cur->content.headlines.push_back(&e);
//       } else {
//         headline_ctx.pop();
//         operator()(e);
//       }
//     }
//   }

//   void operator()(ast::Section &e) {
//     if (headline_ctx.empty()) {
//       doc.content.section.emplace(&e);
//     } else {
//       headline_ctx.top()->content.section.emplace(&e);
//     }
//   }
// };

} // namespace org

} // namespace my
