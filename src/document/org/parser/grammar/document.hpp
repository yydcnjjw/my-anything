#pragma once

#include <stack>

#include <org/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct DocumentClz;
using document_t = x3::rule<DocumentClz, ast::Document>;

BOOST_SPIRIT_DECLARE(document_t);

struct document_ctx_tag;
template<typename Iterator>
struct DocumentCtx {
  std::stack<std::size_t> levels{{0}};
};

template <typename Iterator>
using document_context_type =
    x3::context<document_ctx_tag,
                std::reference_wrapper<DocumentCtx<Iterator>>>;

} // namespace grammar

grammar::document_t const &document();

} // namespace org
} // namespace my
