#include <service/command_service.hpp>

#include <sstream>

#include <app.hpp>

namespace my {

Command::~Command() {
  for (auto &keybind : this->_keybinds) {
    keybind->cmd(nullptr);
  }
}

void Command::add_keybind(keybind_ptr_t const &kb) {
  this->_keybinds.push_back(kb);
}
void Command::remove_keybind(keybind_ptr_t const &kb) {
  std::erase_if(this->_keybinds, [&kb](auto &p) { return &p == &kb; });
}

class CommandCompleteItem : public CompleteItem {
public:
  SHARED_CLS(CommandCompleteItem)

  CommandCompleteItem(Command::ptr_t const &cmd) : _cmd(cmd) {}

  std::string const &desc() override {
    std::stringstream ss;
    ss << this->_cmd->name().c_str();

    auto keybinds{this->_cmd->keybinds()};

    if (!keybinds.empty()) {
      ss << "(";
      auto it{keybinds.begin()};

      ss << (*it++)->keyseq_str();
      while (it != keybinds.end())
        ss << (*it++)->keyseq_str();

      ss << ")";
    }
    this->_desc = ss.str();
    return this->_desc;
  }

  void exec() override { this->_cmd->exec(); }

private:
  Command::ptr_t _cmd;
  std::string _desc;
};

void CommandCompleteBackend::complete(std::string const &text,
                                      complete_list &list) {
  std::ranges::copy(App::get().command_srv().match_command(text) |
                        std::views::filter([](auto const &cmd) {
                          return !cmd->name().starts_with("_");
                        }) |
                        std::views::transform([](auto const &cmd) {
                          return CommandCompleteItem::make(cmd);
                        }),
                    std::back_inserter(list));
}

} // namespace my
