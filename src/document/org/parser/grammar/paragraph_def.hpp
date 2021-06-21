#pragma once

#include <org/parser/grammar/common.hpp>
#include <org/parser/grammar/paragraph.hpp>

namespace my {
namespace org {
namespace grammar {
namespace paragraph {

paragraph_t const paragraph{"paragraph"};
auto const paragraph_def{org::line() - eoi};

} // namespace paragraph

BOOST_SPIRIT_DEFINE(paragraph::paragraph);

struct ParagraphClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::paragraph_t const &paragraph() {
  return grammar::paragraph::paragraph;
}

} // namespace org
} // namespace my
