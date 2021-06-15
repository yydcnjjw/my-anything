#pragma once

#include <boost/spirit/home/x3/directive/no_case.hpp>
#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/block.hpp>

namespace my {
namespace org {

namespace grammar {

namespace block {

using x3::char_;
using x3::eol;
using x3::lit;
using x3::no_case;
using x3::omit;
using x3::string;
using x3::space;

auto const begin{lit("#+begin")};
auto const end{lit("#+end")};
auto const name{+(any - space)};
auto const param{+(any - space)};
auto const param_list{param % blank};

auto name_check = [](auto &ctx) {
  auto &greater_block = x3::_val(ctx);
  auto &name = x3::_attr(ctx);
  x3::_pass(ctx) = greater_block.name == name;
};

auto const greater_begin_line =
    blank_eol_block(begin > lit('_') > name > -(plus_blank > param_list));
auto const greater_end_line =
    blank_eol_block(end > lit('_') > omit[name[name_check]]);
auto const greater_content{*(org::line() - greater_end_line)};
  
greater_block_t const greater_block{"greater_block"};  

// clang-format off
auto const greater_block_def{
  greater_begin_line
    > greater_content >
  greater_end_line
};
// clang-format off

auto const dynamic_begin_line =
  blank_eol_block(begin > lit(':') > plus_blank > name > -(plus_blank > param_list));
auto const dynamic_end_line =
    blank_eol_block(end > lit(':'));
auto const dynamic_content{*(org::line() - dynamic_end_line)};

dynamic_block_t const dynamic_block{"dynamic_block"};

// clang-format off
auto const dynamic_block_def{
  dynamic_begin_line
    > dynamic_content >
  dynamic_end_line
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
