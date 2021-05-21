#include <iostream>
#include <org/org.hpp>

namespace x3 = boost::spirit::x3;

int main(int argc, char *argv[]) {

  using x3::char_;
  std::string s{R"(:b:c:)"};

  auto const tags = ':' > +(char_ - char_(":")) % ':';
  auto const title = (+char_) - tags;

  std::vector<std::string> vec;
  auto r = x3::parse(s.begin(), s.end(), x3::omit[title] > tags, vec);

  for (auto &s : vec) {
    std::cout << s << std::endl;
  }
  std::cout << r << std::endl;
  return 0;
}
