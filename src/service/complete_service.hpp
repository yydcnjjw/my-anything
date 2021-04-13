#pragma once

#include <stack>
#include <variant>

#include <boost/di/extension/injections/lazy.hpp>

#include <core/core.hpp>

#include <service/shortcut_service.hpp>

namespace my {
class CmdLine;
class CompleteItem {
public:
  SHARED_CLS(CompleteItem)

  CompleteItem(std::string const &value) : _value(value), _desc(value) {}

  CompleteItem(std::string const &value, std::string const &desc)
      : _value(value), _desc(desc) {}

  virtual std::string const &value() { return this->_value; }
  virtual std::string const &desc() { return this->_desc; }

protected:
  std::string _value;
  std::string _desc;
};

using complete_list = std::vector<CompleteItem::ptr_t>;

struct CompleteBackend {
  SHARED_CLS(CompleteBackend)

  virtual ~CompleteBackend() = default;
  virtual std::string const &name() const = 0;
  virtual std::string const &complete_prefix() = 0;
  virtual complete_list complete(std::string const &text) = 0;
};

class CompleteService {
public:
  SELF_T(CompleteService)
  using complete_backend_container_t =
      std::map<std::string, CompleteBackend::ptr_t>;
  using complete_fn = std::function<complete_list(std::string const &input)>;
  class CompleteCtx {
  public:
    SHARED_CLS(CompleteCtx)

    CompleteCtx(std::string const &prompt, std::vector<std::string> candidates)
        : _prompt(prompt) {
      std::ranges::copy(candidates | std::views::transform([](auto const &v) {
                          return CompleteItem::make(v);
                        }),
                        std::back_inserter(this->_candidates));
    }

    CompleteCtx(std::string const &prompt, complete_fn const &fn)
        : _prompt(prompt), _candidates_fn(fn) {}

    void input(std::string const &input) { this->_input = input; }
    std::string const &input() const { return this->_input; }

    void value(std::string const &v) {
      this->_value.get_subscriber().on_next(v);
      this->_value.get_subscriber().on_completed();
    }

    void select(std::size_t i) {
      if (this->_candidates.size() > i) {
        this->value(this->_candidates.at(i)->value());
      } else {
        SPDLOG_WARN("Selected non-existent item");
      }
    }

    rx::observable<std::string> value() {
      return this->_value.get_observable();
    }

    auto candidates() {
      if (this->_candidates_fn) {
        this->_candidates = this->_candidates_fn(this->input());
      }

      return this->_candidates |
             std::views::filter([this](auto const &candidate) {
               return candidate->value().starts_with(this->input());
             });
    }

  private:
    std::string _prompt;
    std::string _input;
    complete_fn _candidates_fn;
    complete_list _candidates;
    rx::subjects::subject<std::string> _value;
  };

  struct InteractiveBackend {
    SHARED_CLS(InteractiveBackend)
    using text_changed_ob = rx::observable<std::string>;
    using item_selected_ob = rx::observable<int>;
    virtual ~InteractiveBackend() = default;
    virtual void update_items(CompleteCtx::ptr_t const &) = 0;
    virtual void clear_items() = 0;
    virtual text_changed_ob text_changed() = 0;
    virtual item_selected_ob item_selected() = 0;
  };

  CompleteService(CommandService &command_srv, ShortcutService &shortcut_srv);

  template <typename... Args, typename = std::enable_if_t<!std::conjunction_v<
                                  std::is_same<CompleteCtx::ptr_t, Args>...>>>
  rx::observable<std::string> read_input(Args &&...args) {
    return this->read_input(CompleteCtx::make(std::forward<Args>(args)...));
  }

  complete_backend_container_t const &complete_backends() const {
    return this->_complete_backends;
  }

  std::optional<CompleteBackend::ptr_t>
  get_complete_backends(std::string const &backend) {
    auto it = this->_complete_backends.find(backend);
    if (it != this->_complete_backends.end()) {
      return it->second;
    } else {
      return std::nullopt;
    }
  }

  void add_complete_backend(CompleteBackend::ptr_t const &backend) {
    this->_complete_backends.emplace(backend->name(), backend);
  }

  void remove_complete_backend(CompleteBackend::ptr_t const &backend) {
    this->remove_complete_backend(backend->name());
  }

  void remove_complete_backend(std::string const &name) {
    this->_complete_backends.erase(name);
  }

  void set_interactive_backend(InteractiveBackend::ptr_t const &backend);

private:
  struct InteractiveCtx {
    InteractiveBackend::ptr_t backend;
    rx::composite_subscription text_changed_cs;
    rx::composite_subscription item_selected_cs;

    ~InteractiveCtx() {
      this->text_changed_cs.unsubscribe();
      this->item_selected_cs.unsubscribe();
    }

    void update_items(CompleteCtx::ptr_t const &ctx) {
      if (!this->backend) {
        return;
      }
      this->backend->update_items(ctx);
    }

    void clear_items() {
      if (!this->backend) {
        return;
      }
      this->backend->clear_items();
    }
  };

  complete_backend_container_t _complete_backends;
  std::stack<CompleteCtx::ptr_t> _ctxs;
  InteractiveCtx _interactive_ctx;

  void service_init(CommandService &command_srv, ShortcutService &shortcut_srv);

  std::optional<CompleteCtx::ptr_t> get_ctx_top() {
    if (this->_ctxs.empty()) {
      return std::nullopt;
    } else {
      return this->_ctxs.top();
    }
  }

  void update_items() {
    auto ctx = this->get_ctx_top();
    if (ctx) {
      this->_interactive_ctx.update_items(*ctx);
    } else {
      this->_interactive_ctx.clear_items();
    }
  }

  rx::observable<std::string> read_input(CompleteCtx::ptr_t const &ctx) {
    this->_ctxs.push(ctx);

    this->_interactive_ctx.update_items(ctx);

    return ctx->value();
  }
};

} // namespace my
