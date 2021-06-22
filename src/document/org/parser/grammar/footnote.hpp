#pragma once

#include <org/ast/footnote.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

using footnote_def_t = x3::rule<struct FootnoteDefClz, ast::FootnoteDef>;

BOOST_SPIRIT_DECLARE(footnote_def_t);

} // namespace grammar

grammar::footnote_def_t const &footnote_def();

} // namespace org

} // namespace my
