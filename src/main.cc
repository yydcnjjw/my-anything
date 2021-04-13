#include <xapian.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// #include "support.h"
// #include <cppjieba/Jieba.hpp>

// using namespace std;
// std::string const JIEBA_PATH{
//     "/home/yydcnjjw/workspace/code/hack/qt-widgets-test/cppjieba/dict"};
// std::string const DICT_PATH = JIEBA_PATH + "/jieba.dict.utf8";
// std::string const HMM_PATH{JIEBA_PATH + "/hmm_model.utf8"};
// std::string const USER_DICT_PATH{JIEBA_PATH + "/user.dict.utf8"};
// std::string const IDF_PATH{JIEBA_PATH + "/idf.utf8"};
// std::string const STOP_WORD_PATH{JIEBA_PATH + "/stop_words.utf8"};

// void test() {
//   cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH,
//                         STOP_WORD_PATH);

//   auto sentence{
//       "Not all Xapian 测试functionality is yet documented in this guide, so "
//       "once "
//       "you’ve gone through it you may wish to look at our online API "
//       "documentation 我 and also at some of the additional help available on
//       " "the " "Xapian wiki. 这是杀"};

//   std::vector<std::string> words;
//   jieba.Cut(sentence, words);

//   for (auto const &word : words) {
//     std::cout << word << std::endl;
//   }

//   std::cout << "xapain -----" << std::endl;

//   Xapian::TermGenerator termgenerator;
//   //  termgenerator.set_stemmer(Xapian::Stem("en"));

//   Xapian::Document doc;
//   termgenerator.set_document(doc);

//   // for (auto const &word : words) {
//   //   termgenerator.index_text(word);
//   // }

//   termgenerator.index_text(sentence);

//   for (auto it{doc.termlist_begin()}; it != doc.termlist_end(); ++it) {
//     std::cout << *it << std::endl;
//   }
// }

#include <app.hpp>

int main(int argc, char *argv[]) {
  return my::App::run(argc, argv);
}
