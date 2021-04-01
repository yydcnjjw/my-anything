#include <service/complete_service.hpp>

#include <boost/algorithm/string.hpp>

#include <app.hpp>
#include <service/command_service.hpp>

namespace {
using namespace my;

class TextCompleteItem : public CompleteItem {
public:
  SHARED_CLS(TextCompleteItem)

  TextCompleteItem(std::string const &text, std::string const &desc)
      : _text(text), _desc(desc) {}

  std::string const &desc() override { return this->_desc; }

  void exec() override { App::get().cmdline().set_text(this->_text); }

private:
  std::string _text;
  std::string _desc;
};

class BackendCompleteBackend : public CompleteBackend {
public:
  SHARED_CLS(BackendCompleteBackend)

  BackendCompleteBackend(CompleteService *complete_srv)
      : _complete_srv(complete_srv) {}

  std::string const &name() const override { return this->_name; }

  std::string const &complete_prefix() override {
    return this->_complete_prefix;
  }

  void complete(std::string const &text, complete_list &list) override {
    std::ranges::copy(this->_complete_srv->complete_backends() |
                          std::views::values |
                          std::views::filter([&text](auto const &backend) {
                            return backend->complete_prefix().starts_with(text);
                          }) |
                          std::views::transform([](auto const &backend) {
                            return TextCompleteItem::make(
                                backend->complete_prefix(),
                                (boost::format("%s(%s)") %
                                 backend->complete_prefix() % backend->name())
                                    .str());
                          }),
                      std::back_inserter(list));
  }

private:
  CompleteService *_complete_srv;
  std::string _name{"backend"};
  std::string _complete_prefix{""};
};

// class FileCompleteBackend : public CompleteBackend {
// public:
//   std::string const &name() const override {}

//   std::string const &complete_prefix() override {}
//   void complete(std::string const &text, complete_list &list) override {}

// private:
// };

} // namespace

namespace my {

CompleteService::CompleteService() {
  this->add_complete_backend(BackendCompleteBackend::make(this));
  this->add_complete_backend(CommandCompleteBackend::make());
}

void CompleteService::disptach(std::string const &input, complete_list &list) {
  auto text = boost::trim_copy(input);
  for (auto const &backend :
       this->_complete_backends | std::views::values |
           std::views::filter([&text](auto const &backend) {
             return text.starts_with(backend->complete_prefix());
           })) {
    backend->complete(
        boost::trim_copy(text.substr(backend->complete_prefix().size())), list);
  }
}

} // namespace my
