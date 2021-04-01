#pragma once

#include <core/core.hpp>
#include <service/complete_service.hpp>

namespace my {
class KeyBind;
class Command {
public:
  SHARED_CLS(Command)

  using executor = std::function<void()>;
  using keybind_ptr_t = std::shared_ptr<KeyBind>;
  using keybind_container_t = std::vector<keybind_ptr_t>;

  template <typename _Executor>
  Command(std::string const &name, _Executor &&executor,
          std::string const &desc = {})
      : _name(name), _ex(std::forward<_Executor>(executor)), _desc(desc) {}

  ~Command();

  template <typename... Args> void exec(Args &&...args) {
    _ex(std::forward<Args>(args)...);
  }

  std::string const &name() const { return this->_name; }
  std::string const &desc() const { return this->_desc; }
  keybind_container_t const &keybinds() const { return this->_keybinds; }

private:
  std::string _name;
  std::string _desc;
  executor _ex;
  keybind_container_t _keybinds;

  friend class KeyBind;
  void add_keybind(keybind_ptr_t const &);
  void remove_keybind(keybind_ptr_t const &);
};

class CommandService {
public:
  UNIQUE_CLS(CommandService)
  using command_container_t = std::map<std::string, Command::ptr_t>;

  CommandService() {}

  template <typename... Args, typename = std::enable_if_t<!std::conjunction_v<
                                  std::is_same<Command::ptr_t, Args>...>>>
  self_t &add(Args &&...args) {
    return this->add(Command::make(std::forward<Args>(args)...));
  }

  self_t &add(Command::ptr_t const &cmd) {
    if (!this->_commands.emplace(cmd->name(), cmd).second) {
      SPDLOG_WARN("Command {} has been registered", cmd->name());
    }
    return *this;
  }

  std::optional<Command::ptr_t> get_command(std::string const &name) {
    if (this->_commands.count(name)) {
      return this->_commands.at(name);
    } else {
      return std::nullopt;
    }
  }

  auto match_command(std::string const &text) {
    return this->_commands | std::views::filter([text](auto const &kv) {
             return !text.empty() && kv.first.starts_with(text);
           }) |
           std::views::transform([](auto const &kv) { return kv.second; });
  }

  command_container_t const &commands() { return this->_commands; }

private:
  command_container_t _commands;
};

class CommandCompleteBackend : public CompleteBackend {
public:
  SHARED_CLS(CommandCompleteBackend)
  
  std::string const &name() const override { return this->_name; }

  std::string const &complete_prefix() override {
    return this->_complete_prefix;
  }
  void complete(std::string const &text, complete_list &list) override;

private:
  std::string _name{"command"};
  std::string _complete_prefix{">"};
};

} // namespace my
