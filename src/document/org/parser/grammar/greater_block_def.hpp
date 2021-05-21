#pragma once

#include <boost/spirit/home/x3/directive/no_case.hpp>
#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/greater_block.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::eol;
using x3::lit;
using x3::no_case;
using x3::omit;
using x3::string;
namespace greater_block {

auto const begin{lit("#+begin_")};
auto const end{lit("#+end_")};
auto const name{+(char_ - char_(' ') - eol)};
auto const param{+(char_ - char_(' ') - eol)};
auto const param_list{param % +char_(' ')};
auto const content{*(org::line() - end)};

greater_block_t const greater_block{"greater_block"};
auto const greater_block_def = begin > name >
                               ((+lit(' ') > param_list > eol) | eol) > content
                               > end > omit[name] > eol;
} // namespace greater_block

BOOST_SPIRIT_DEFINE(greater_block::greater_block);

struct GreaterBlockClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::greater_block_t const &greater_block() {
  return grammar::greater_block::greater_block;
}

} // namespace org
} // namespace my
