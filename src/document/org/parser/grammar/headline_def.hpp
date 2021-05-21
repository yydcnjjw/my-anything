#pragma once

#include <org/parser/grammar/headline.hpp>

namespace my {
namespace org {

namespace grammar {

namespace ascii = x3::ascii;

using ascii::blank;
using x3::char_;
using x3::eol;
using x3::lexeme;
using x3::omit;

namespace headline {
struct StartsClz : x3::annotate_on_success, error_handler_base {};
x3::rule<StartsClz, std::string> const starts{"starts"};
auto const starts_def = +char_('*') > omit[+blank];

struct KeywordClz : x3::annotate_on_success, error_handler_base {};
x3::rule<struct KeywordClz, std::string> const keyword{"keyword"};
auto const keyword_def{+char_("A-Z") > omit[+blank]};

struct PriorityClz : x3::annotate_on_success, error_handler_base {};
x3::rule<PriorityClz, char> const priority{"priority"};
auto const priority_def{lexeme["[#" > char_("a-zA-Z") > ']'] > omit[*blank]};

struct TitleClz : x3::annotate_on_success, error_handler_base {};
x3::rule<TitleClz, std::string> const title{"title"};
auto const title_def{(*(char_ - eol))};

BOOST_SPIRIT_DEFINE(headline::starts, headline::keyword, headline::priority,
                    headline::title);

headline_t const headline{"headline"};
auto const headline_def{starts > -keyword > -priority > title > eol};

} // namespace headline

BOOST_SPIRIT_DEFINE(headline::headline);

struct HeadlineClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::headline_t const &headline() { return grammar::headline::headline; }

} // namespace org
} // namespace my
