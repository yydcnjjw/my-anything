#pragma once

#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/vector.hpp>
#include <org/ast/types.hpp>

namespace my {

namespace org {
namespace ast {

struct BaseData : x3::position_tagged {};

struct ElementData : BaseData {};

struct GreaterElementData : ElementData {};

struct ObjectData : BaseData {};

struct RecursiveObject : ObjectData {};

struct ContainerObject : RecursiveObject {};

namespace detail {

template <typename... Ts>
using cat = decltype(std::tuple_cat(std::declval<Ts>()...));

template <typename TResult, typename... Ts> struct flatten_variant;

template <typename TResult> struct flatten_variant<TResult> {
  using type = TResult;
};

template <typename TResult, typename T, typename... TOther>
struct flatten_variant<TResult, T, TOther...> {
  using type = cat<TResult, std::tuple<T>,
                   typename flatten_variant<TResult, TOther...>::type>;
};

template <typename TResult, typename... Ts, typename... TOther>
struct flatten_variant<TResult, x3::variant<Ts...>, TOther...> {
  using type = cat<TResult, typename flatten_variant<std::tuple<>, Ts...>::type,
                   typename flatten_variant<TResult, TOther...>::type>;
};

template <typename T> struct to_variant;

template <typename... Ts> struct to_variant<std::tuple<Ts...>> {
  using type = x3::variant<Ts...>;
};
} // namespace detail

template <typename T>
using flat_variant_t = typename detail::to_variant<
    typename detail::flatten_variant<std::tuple<>, T>::type>::type;

} // namespace ast

} // namespace org

} // namespace my
