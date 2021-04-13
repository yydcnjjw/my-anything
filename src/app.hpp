#pragma once

#include <QApplication>
#include <boost/di.hpp>
#include <service/command_service.hpp>
#include <service/complete_service.hpp>
#include <service/shortcut_service.hpp>

namespace my {
using argc_t = int &;
using argv_t = char **;
struct Args {
  argc_t argc;
  argv_t argv;
  Args(argc_t argc, argv_t argv) : argc(argc), argv(argv) {}
};

class App : public QApplication {
public:
  App(CommandService &command_srv, ShortcutService &shortcut_srv,
      CompleteService &complete_srv);

  static int run(argc_t argc, argv_t argv);

  std::optional<QKeyEvent *> const &cur_key_ev() const {
    return this->_cur_key_ev;
  }

  bool notify(QObject *, QEvent *) override;

private:
  ShortcutService _shortcut_srv;
  std::optional<QKeyEvent *> _cur_key_ev;
  

  void commands_init(CommandService &command_srv,
                     CompleteService &complete_srv);
  void keymap_init(ShortcutService &srv);
};

} // namespace my
