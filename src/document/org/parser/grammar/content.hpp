#pragma once

#include <org/parser/ast/ast.hpp>
#include <org/parser/config.hpp>

namespace my {
namespace org {
namespace grammar {

struct ContentClz;
using content_t = x3::rule<ContentClz, ast::Content>;

BOOST_SPIRIT_DECLARE(content_t);

} // namespace grammar

grammar::content_t const &content();

} // namespace org
} // namespace my
