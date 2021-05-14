#pragma once

#include <boost/spirit/include/support_multi_pass.hpp>
#include <org/parser/error_handler.hpp>
#include <org/parser/grammar/document.hpp>
#include <org/parser/types.hpp>

namespace my {
namespace org {

using streambuf_iterator_type = std::istreambuf_iterator<char>;

using stream_iterator_type = boost::spirit::multi_pass<streambuf_iterator_type>;

using iterator_type = std::string::const_iterator;

using phrase_context_type =
    x3::phrase_parse_context<x3::ascii::blank_type>::type;

template <typename Iterator>
using context_t =
    x3::context<error_handler_tag,
                std::reference_wrapper<error_handler<Iterator>>
                >;
} // namespace org

} // namespace my
