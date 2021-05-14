#include <iostream>
#include <org/org.hpp>

namespace x3 = boost::spirit::x3;

int main(int argc, char *argv[]) {
  std::string s{R"(
fdaffd

fdaffd
fasdfsf
* headline1
section2
** headline2
section3
)"};

  auto empty_line_print = [] { std::cout << "empty line" << std::endl; };

  auto headline_print = [](auto const &ctx) {
    std::cout << ">>>headline:" << std::endl;
    std::cout << x3::_attr(ctx) << std::endl;
    std::cout << "<<<headline" << std::endl;
  };

  auto section_print = [](auto const &ctx) {
    std::cout << ">>>section:" << std::endl;
    for (auto v : x3::_attr(ctx)) {
      std::cout << v;
    }
    std::cout << "<<<section" << std::endl;
  };

  auto const empty_line = +x3::space;

  auto const line = +(x3::char_ - x3::eol) > x3::eol;
  auto const headline =
      +x3::char_('*') > +x3::blank > *(x3::char_ - x3::eol) > x3::eol;

  auto const section = +(empty_line[empty_line_print] | (line - headline));

  auto r = x3::parse(s.begin(), s.end(),
                     x3::eps > headline[headline_print] |
                         section[section_print] > x3::eoi);
  std::cout << r << std::endl;
  return 0;
}
