#pragma once

#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/adapted.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_category.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/type_index.hpp>

namespace my {
namespace org {
namespace generator {

template <typename Out, typename T>
inline void print_ast(Out &out, T const &val);

namespace detail {

using namespace boost::spirit::x3;

template <typename Out, typename T>
inline void print_attribute(Out &out, T const &val, traits::plain_attribute) {
  out << val;
}

template <typename Out, typename T>
inline void print_attribute(Out &out, T const &val, traits::tuple_attribute) {
  using Indices = boost::mpl::range_c<unsigned, 0,
                                      boost::fusion::result_of::size<T>::value>;
  bool first = true;
  out << '{';
  out << "\"type\": \"" << boost::typeindex::type_id<T>().pretty_name() << "\",";
  boost::fusion::for_each(Indices{}, [&out, &val,
                                      &first]<typename Index>(Index const &) {
    if (!first) {
      out << ',';
    }
    first = false;

    out << "\""
        << boost::fusion::extension::struct_member_name<T, Index::value>::call()
        << "\"";
    out << ": ";
    print_ast(out, boost::fusion::at<Index>(val));
  });
  out << '}';
}

template <typename T>
struct is_string : public std::disjunction<
                       std::is_same<char *, typename std::decay_t<T>>,
                       std::is_same<const char *, typename std::decay_t<T>>,
                       std::is_same<std::string, typename std::decay_t<T>>> {};

struct string_container_tag {};
struct normal_container_tag {};

template <typename T, typename Enable = void>
struct container_category : boost::mpl::identity<normal_container_tag> {};

template <typename T>
struct container_category<T, std::enable_if_t<is_string<T>::value>>
    : boost::mpl::identity<string_container_tag> {};

template <typename Out, typename T>
inline void print_container(Out &out, T const &val, string_container_tag) {
  out << "\"";
  for (auto ch : val) {
    switch (ch) {
    case '\n':
      out << "\\\\n";
      break;
    case '\r':
      out << "\\\\r";
      break;
    case '\t':
      out << "\\\\t";
      break;
    case '\v':
      out << "\\\\v";
      break;
    case '\f':
      out << "\\\\f";
      break;
    default:
      out << ch;
      break;
    }
    // switch (ch) {
    // case '\n':
    //   out << "\\n";
    //   break;
    // case '\r':
    //   out << "\\r";
    //   break;
    // case '\t':
    //   out << "\\t";
    //   break;
    // case '\v':
    //   out << "\\v";
    //   break;
    // case '\f':
    //   out << "\\f";
    //   break;
    // default:
    //   out << ch;
    //   break;
    // }
    
  }
  out << "\"";
}

template <typename Out, typename T>
inline void print_container(Out &out, T const &val, normal_container_tag) {
  out << '[';
  if (!traits::is_empty(val)) {
    bool first = true;
    typename traits::container_iterator<T const>::type iend = traits::end(val);
    for (typename traits::container_iterator<T const>::type i =
             traits::begin(val);
         !traits::compare(i, iend); traits::next(i)) {
      if (!first)
        out << ", ";
      first = false;
      print_ast(out, traits::deref(i));
    }
  }
  out << ']';
}

template <typename Out, typename T>
inline void print_attribute(Out &out, T const &val,
                            traits::container_attribute) {
  print_container(out, val, typename container_category<T>::type{});
}

template <typename Out, typename T>
inline void print_attribute(Out &out, T const &val,
                            traits::optional_attribute) {
  if (val)
    print_ast(out, *val);
  else
    out << "null";
}

template <typename Out, typename T>
inline void print_attribute(Out &out, T const &val, traits::variant_attribute) {
  boost::apply_visitor([&out](auto const &v) { print_ast(out, v); }, val);
}

} // namespace detail

template <typename Out, typename T>
inline void print_ast(Out &out, T const &val) {
  detail::print_attribute(
      out, val, typename detail::traits::attribute_category<T>::type{});
}

} // namespace generator
} // namespace org
} // namespace my
