#pragma once

#include <list>

#include <boost/fusion/adapted/adt/adapt_adt.hpp>
#include <boost/fusion/include/adapt_adt.hpp>

#include <org/parser/ast/data_type.hpp>
#include <org/parser/ast/section.hpp>

namespace my {
namespace org {
namespace ast {

struct Headline;

struct Content : x3::position_tagged {
  boost::optional<Section> section;
  std::list<Headline> headlines;
};

std::ostream &operator<<(std::ostream &os, Content const &v);
  
} // namespace ast
} // namespace org
} // namespace my

#include <org/parser/ast/impl/content.ipp>
