#pragma once

#include <iostream>
#include <org/parser/annotate_on_success.hpp>
#include <org/parser/types.hpp>

namespace my {
namespace org {

template <typename Iterator> using error_handler = x3::error_handler<Iterator>;

using error_handler_tag = x3::error_handler_tag;

struct error_handler_base {
  template <typename Iterator, typename Exception, typename Context>
  x3::error_handler_result on_error(Iterator &first, Iterator const &last,
                                    Exception const &x,
                                    Context const &context) {
    std::string message = "Error! Expecting: " + x.which() + " here:";
    auto &error_handler = x3::get<error_handler_tag>(context).get();
    error_handler(x.where(), message);
    return x3::error_handler_result::fail;
  }
};

} // namespace org
} // namespace my
