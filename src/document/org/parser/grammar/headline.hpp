#pragma once

#include <org/ast/headline.hpp>
#include <org/parser/config.hpp>
#include <org/parser/grammar/common.hpp>

namespace my {

namespace org {
namespace grammar {

using headline_t = x3::rule<struct HeadlineClz, ast::Headline, true>;

BOOST_SPIRIT_DECLARE(headline_t);

auto constexpr headline_header = (+char_('*') > ' ' > *(any - eol) > (eol | eoi));

} // namespace grammar

grammar::headline_t const &headline();

} // namespace org

} // namespace my
