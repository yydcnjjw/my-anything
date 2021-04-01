#pragma once

#include <QApplication>
#include <service/command_service.hpp>
#include <service/complete_service.hpp>
#include <service/shortcut_service.hpp>
#include <ui/cmdline.hpp>

namespace my {

class App : public QApplication {
public:
  App(int argc, char **argv);

  static App &get() {
    return *reinterpret_cast<App *>(QApplication::instance());
  }

  bool notify(QObject *, QEvent *) override;

  int run();

  CommandService &command_srv() { return *this->_command_srv; }
  ShortcutService &shortcut_srv() { return *this->_shortcut_srv; }
  CompleteService &complete_srv() { return *this->_complete_srv; }

  std::optional<QKeyEvent *> const &cur_key_ev() const {
    return this->_cur_key_ev;
  }
  CmdLine &cmdline() { return *this->_cmdline; }

private:
  CmdLine::ptr_t _cmdline;
  CommandService::ptr_t _command_srv;
  ShortcutService::ptr_t _shortcut_srv;
  CompleteService::ptr_t _complete_srv;

  std::optional<QKeyEvent *> _cur_key_ev;

  void service_init();
  void commands_init();
  void keymap_init();
};

} // namespace my
