#pragma once

#include <org/parser/ast/types.hpp>

namespace my {

namespace org {
namespace ast {

struct BaseData : x3::position_tagged {};

struct ElementData : BaseData {};

struct GreaterElementData : ElementData {};

struct ObjectData : BaseData {};

struct RecursiveObject : ObjectData {};

struct ContainerObject : RecursiveObject {};

} // namespace ast

} // namespace org

} // namespace my
