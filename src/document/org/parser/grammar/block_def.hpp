#pragma once

#include <boost/spirit/home/x3/directive/no_case.hpp>
#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/block.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::eol;
using x3::lit;
using x3::no_case;
using x3::omit;
using x3::string;

namespace block {

auto const begin{lit("#+begin")};
auto const end{lit("#+end")};
auto const name{+(char_ - char_(' ') - eol)};
auto const param{+(char_ - char_(' ') - eol)};
auto const param_list{param % +char_(' ')};
auto const content{*(org::line() - end)};

auto name_check = [](auto &ctx) {
  auto &greater_block = x3::_val(ctx);
  auto &name = x3::_attr(ctx);
  x3::_pass(ctx) = greater_block.name == name;
};
  
greater_block_t const greater_block{"greater_block"};  

// clang-format off
auto const greater_block_def{
  begin > lit('_') > name > ((+lit(' ') > param_list > eol) | eol)
    > content >
  end > lit('_') > omit[name[name_check]] > eol
};
// clang-format off
  
dynamic_block_t const dynamic_block{"dynamic_block"};

// clang-format off
auto const dynamic_block_def{
  begin > lit(':') > +lit(' ') > name > ((+lit(' ') > param_list > eol) | eol)
    > content >
  end > lit(':') >> *lit(' ') > eol
};
// clang-format off

} // namespace block

struct GreaterBlockClz : x3::annotate_on_success, error_handler_base {};
struct DynamicBlockClz : x3::annotate_on_success, error_handler_base {};
  
BOOST_SPIRIT_DEFINE(block::greater_block, block::dynamic_block);

} // namespace grammar

grammar::greater_block_t const &greater_block() {
  return grammar::block::greater_block;
}

grammar::dynamic_block_t const &dynamic_block() {
  return grammar::block::dynamic_block;
}

} // namespace org
} // namespace my
