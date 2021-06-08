#pragma once

#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/content.hpp>

namespace my {
namespace org {

namespace grammar {

namespace ascii = x3::ascii;

using ascii::blank;
using x3::char_;
using x3::eol;
using x3::lexeme;
using x3::omit;
using x3::raw;
  
namespace headline {
struct StarsClz : x3::annotate_on_success, error_handler_base {};
auto const stars = x3::rule<StarsClz, std::string>{"stars"} =
    +char_('*') > omit[+blank];

struct KeywordClz : x3::annotate_on_success, error_handler_base {};
auto const keyword = x3::rule<struct KeywordClz, std::string>{"keyword"} =
    +char_("A-Z") > omit[+blank];

struct PriorityClz : x3::annotate_on_success, error_handler_base {};
auto const priority = x3::rule<PriorityClz, char>{"priority"} =
    lexeme["[#" > char_("a-zA-Z") > ']'] > omit[*blank];

struct TagsClz : x3::annotate_on_success, error_handler_base {};
auto const tags = x3::rule<TagsClz, std::vector<std::string>>{
    "tags"} = ':' > *(char_ - char_(" :")) % ':';

struct TitleClz : x3::annotate_on_success, error_handler_base {};
auto const title = x3::rule<TitleClz, std::string>{"title"} =
    (+(char_ - eol)) - tags;

headline_t const headline{"headline"};

auto const headline_def{stars > -keyword > -priority > -title > eol >
                        org::content()};

} // namespace headline

BOOST_SPIRIT_DEFINE(headline::headline);

struct HeadlineClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::headline_t const &headline() { return grammar::headline::headline; }

} // namespace org
} // namespace my
