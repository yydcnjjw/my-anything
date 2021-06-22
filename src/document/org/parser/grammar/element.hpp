#pragma once

#include <org/parser/grammar/block.hpp>
#include <org/parser/grammar/drawer.hpp>
#include <org/parser/grammar/footnote.hpp>
#include <org/parser/grammar/paragraph.hpp>

namespace my {
namespace org {
namespace grammar {

auto const element = org::paragraph();
  
auto const greater_element = org::greater_block() | org::dynamic_block() |
                             org::drawer() | org::footnote_def();

}
} // namespace org
} // namespace my
