#include <service/complete_service.hpp>

#include <boost/algorithm/string.hpp>

#include <app.hpp>
#include <ui/cmdline.hpp>

namespace {
using namespace my;

class BackendCompleteBackend : public CompleteBackend {
public:
  SHARED_CLS(BackendCompleteBackend)

  BackendCompleteBackend(CompleteService *complete_srv)
      : _complete_srv(complete_srv) {}

  std::string const &name() const override { return this->_name; }

  std::string const &complete_prefix() override {
    return this->_complete_prefix;
  }

  complete_list complete(std::string const &text) override {
    complete_list list;
    std::ranges::copy(this->_complete_srv->complete_backends() |
                          std::views::values |
                          std::views::filter([&text](auto const &backend) {
                            auto prefix{backend->complete_prefix()};
                            return prefix != text && prefix.starts_with(text);
                          }) |
                          std::views::transform([](auto const &backend) {
                            return CompleteItem::make(
                                backend->complete_prefix(),
                                (boost::format("%s(%s)") %
                                 backend->complete_prefix() % backend->name())
                                    .str());
                          }),
                      std::back_inserter(list));
    return list;
  }

private:
  CompleteService *_complete_srv;
  std::string _name{"backend"};
  std::string _complete_prefix{""};
};

class DynamicCompleteBackend : public CompleteBackend {
public:
  SHARED_CLS(DynamicCompleteBackend)

  DynamicCompleteBackend(CompleteService *complete_srv)
      : _complete_srv(complete_srv) {}

  std::string const &name() const override { return this->_name; }

  std::string const &complete_prefix() override {
    return this->_complete_prefix;
  }

  complete_list complete(const std::string &input) override {
    complete_list list;
    auto text = boost::trim_copy(input);
    for (auto v : this->_complete_srv->complete_backends() |
                      std::views::filter([this](auto const &kv) {
                        return kv.first != this->name();
                      })

                      | std::views::values |
                      std::views::filter([&text](auto const &backend) {
                        return text.starts_with(backend->complete_prefix());
                      }) |
                      std::views::transform([&text](auto const &backend) {
                        return backend->complete(boost::trim_copy(
                            text.substr(backend->complete_prefix().size())));
                      })) {
      list.insert(list.end(), v.begin(), v.end());
    }
    return list;
  }

  CompleteService *_complete_srv;
  std::string _name{"dynamic"};
  std::string _complete_prefix{""};
};

class FileCompleteBackend : public CompleteBackend {
public:
  SHARED_CLS(FileCompleteBackend)
  std::string const &name() const override { return this->_name; }

  std::string const &complete_prefix() override {
    return this->_complete_prefix;
  }

  complete_list complete(std::string const &text) override {
    complete_list list;
    fs::path input;

    if (text.empty()) {
      input = fs::absolute("/");
    } else {
      input = fs::absolute(text);
    }

    auto dir = input.parent_path();
    auto filename = input.filename();

    if (!fs::is_directory(dir)) {
      return list;
    }

    for (auto &subpath : fs::directory_iterator(
             dir, fs::directory_options::skip_permission_denied)) {
      auto path = subpath.path().string();
      auto subfilename = subpath.path().filename().string();
      if (subfilename.starts_with(filename.string())) {
        std::string s;
        if (subpath.is_directory()) {
          s = (boost::format("%s/") % path).str();
        } else {
          s = (boost::format("%s") % path).str();
        }

        list.push_back(CompleteItem::make(s, path));
      }
    }
    return list;
  }

private:
  std::string _name{"file"};
  std::string _complete_prefix{"file://"};
};

} // namespace

namespace my {

CompleteService::CompleteService(CommandService &command_srv,
                                 ShortcutService &shortcut_srv) {
  this->service_init(command_srv, shortcut_srv);
}

void CompleteService::service_init(CommandService &command_srv,
                                   ShortcutService &shortcut_srv) {
  this->add_complete_backend(DynamicCompleteBackend::make(this));
  this->add_complete_backend(BackendCompleteBackend::make(this));
  this->add_complete_backend(FileCompleteBackend::make());
}

void CompleteService::set_interactive_backend(
    InteractiveBackend::ptr_t const &backend) {

  auto text_changed_cs = backend->text_changed().subscribe(
      [this, &backend](std::string const &input) {
        auto ctx = this->get_ctx_top();

        if (!ctx) {
          return;
        }

        if ((*ctx)->input() == input) {
          return;
        }

        (*ctx)->input(input);

        this->_interactive_ctx.update_items(*ctx);
      });

  auto item_selected_cs = backend->item_selected().subscribe([this](auto i) {
    auto ctx = this->get_ctx_top();
    if (!ctx) {
      return;
    }

    (*ctx)->select(i);

    this->_ctxs.pop();

    this->update_items();
  });

  this->_interactive_ctx = {backend, text_changed_cs, item_selected_cs};
}

} // namespace my
