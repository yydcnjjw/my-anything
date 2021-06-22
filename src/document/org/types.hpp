#pragma once

#define BOOST_SPIRIT_X3_UNICODE
#include <boost/spirit/home/x3.hpp>

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

namespace my {
namespace org {

namespace x3 = boost::spirit::x3;
template <typename T> using optional = boost::optional<T>;

#define NULL_OPT boost::none;

} // namespace org
} // namespace my
