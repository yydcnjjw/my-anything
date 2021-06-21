#pragma once

#include <org/ast/headline.hpp>
#include <org/parser/config.hpp>

namespace my {

namespace org {
namespace grammar {

using headline_t = x3::rule<struct HeadlineClz, ast::Headline, true>;

BOOST_SPIRIT_DECLARE(headline_t);

} // namespace grammar

grammar::headline_t const &headline();

} // namespace org

} // namespace my
