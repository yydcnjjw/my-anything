#pragma once

#include <boost/spirit/home/x3/directive/no_case.hpp>
#include <org/parser/grammar/greater_block.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::lit;
using x3::no_case;

namespace greater_block {

auto const begin{no_case[lit("#+begin_")]};
auto const end{no_case[lit("#+end_")]};
auto const name{no_case[*(char_ - char_(' '))]};
auto const content{no_case[*(char_ - char_(' '))]};

greater_block_t const greater_block{"greater_block"};
auto const greater_block_def = begin > name > end;

BOOST_SPIRIT_DEFINE(greater_block);

} // namespace greater_block

struct GreaterBlockClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::greater_block_t const &greater_block() {
  return grammar::greater_block::greater_block;
}

} // namespace org
} // namespace my
