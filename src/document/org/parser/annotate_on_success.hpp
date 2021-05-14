#pragma once

#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/context.hpp>
#include <boost/spirit/home/x3/support/utility/lambda_visitor.hpp>

namespace boost {
namespace spirit {
namespace x3 {
///////////////////////////////////////////////////////////////////////////
//  The on_success handler tags the AST with the iterator position
//  for error handling.
//
//  The on_success handler also ties the AST to a vector of iterator
//  positions for the purpose of subsequent semantic error handling
//  when the program is being compiled. See x3::position_cache in
//  x3/support/ast.
//
//  We'll ask the X3's error_handler utility to do these.
///////////////////////////////////////////////////////////////////////////

struct annotate_on_success {
  template <typename Iterator, typename Context, typename... Types>
  inline void on_success(Iterator const &first, Iterator const &last,
                         variant<Types...> &ast, Context const &context) {
    ast.apply_visitor(x3::make_lambda_visitor<void>(
        [&](auto &node) { this->on_success(first, last, node, context); }));
  }

  template <typename T, typename Iterator, typename Context>
  inline void on_success(Iterator const &first, Iterator const &last, T &ast,
                         Context const &context) {
    // FIX for https://github.com/boostorg/spirit/issues/526
    auto &error_handler = x3::get<error_handler_tag>(context).get();
    error_handler.tag(ast, first, last);
  }
};
} // namespace x3
} // namespace spirit
} // namespace boost
