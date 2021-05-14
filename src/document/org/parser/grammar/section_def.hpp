#pragma once

#include <org/parser/grammar/headline.hpp>
#include <org/parser/grammar/section.hpp>

namespace my {
namespace org {

namespace grammar {

using x3::char_;
using x3::eoi;
using x3::eol;

namespace section {

section_t const section{"section"};

auto const line{*(char_ - eol) > eol};

auto section_action = [](auto &ctx) {
  x3::_val(ctx).content += x3::_attr(ctx) + '\n';
};
auto const section_def{+(line // - headline()
                         - eoi)[section_action]};

BOOST_SPIRIT_DEFINE(section);

} // namespace section

struct SectionClz : x3::annotate_on_success, error_handler_base {};

} // namespace grammar

grammar::section_t const &section() { return grammar::section::section; }

} // namespace org
} // namespace my
