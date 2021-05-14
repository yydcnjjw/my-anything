#pragma once

#include <fstream>
#include <regex>
#include <sstream>

#include <nnet_language_identifier.h>

#include <core/core.hpp>

namespace my {

class Document {
public:
  SHARED_PTR_T(Document)

  Document(std::string const &main_language) : _main_language(main_language) {}

  std::string const &main_language() const { return this->_main_language; }

protected:
private:
  std::string _main_language;
};

using chrome_lang_id::NNetLanguageIdentifier;

class DocumentBuilder {
public:
  using extension_t = fs::path;

  struct DataStream {
    using stream_t = std::shared_ptr<std::istream>;
    using size_t = std::size_t;

    SHARED_CLS(DataStream)

    DataStream(std::string const &s)
        : DataStream{std::make_shared<std::istringstream>(s)} {}

    DataStream(fs::path const &p)
        : DataStream{std::make_shared<std::ifstream>(p)} {}

    DataStream(stream_t const &is) : _is{is}, _buf(4096, 0) {
      // this->stream().rdbuf()->pubsetbuf(this->_buf.data(),
      // this->_buf.size());
    }

    stream_t::element_type &stream() { return *this->_is; }

    auto begin() { return std::istreambuf_iterator<char>(this->stream()); }

    auto end() { return std::istreambuf_iterator<char>(); }

  private:
    stream_t _is;
    std::string _buf;
  };

  DocumentBuilder(NNetLanguageIdentifier &lang_ident)
      : _lang_ident(lang_ident) {}

  Document::ptr_t from(fs::path const &p) {
    return this->from(DataStream::make(p), p.extension());
  }

  Document::ptr_t from(std::istream &s, extension_t const &ext) {
    auto iss = std::make_shared<std::istringstream>();
    iss->basic_ios<char>::rdbuf(s.rdbuf());
    return this->from(DataStream::make(iss), ext);
  }

  Document::ptr_t from(std::string const &s, extension_t const &ext) {
    return this->from(DataStream::make(s), ext);
  }

private:
  NNetLanguageIdentifier &_lang_ident;

  Document::ptr_t from(DataStream::ptr_t const &data, extension_t const &ext) {
    this->parse(data);
    return nullptr;
  }

  static void print(double v) { std::cout << v << std::endl; }

  void parse(DataStream::ptr_t const &data) {
  }
};

} // namespace my
