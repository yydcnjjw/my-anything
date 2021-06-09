#pragma once

#include <org/parser/grammar/content.hpp>
#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

content_t const content{"content"};

auto const content_def = -org::section() > *org::headline();

BOOST_SPIRIT_DEFINE(content);

struct ContentClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::content_t const &content() { return grammar::content; }

} // namespace org
} // namespace my
