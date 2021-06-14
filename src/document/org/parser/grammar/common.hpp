#pragma once

#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using line_t = x3::rule<struct LineClz, std::string>;
BOOST_SPIRIT_DECLARE(line_t);
  
auto const word = x3::alnum | x3::char_('_');
auto const any = x3::char_;

} // namespace grammar

grammar::line_t const &line();

} // namespace org
} // namespace my
