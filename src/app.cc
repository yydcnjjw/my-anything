#include <QKeyEvent>
#include <app.hpp>

namespace my {

App::App(int argc, char **argv) : QApplication(argc, argv) {
  spdlog::set_level(spdlog::level::debug);
}

int App::run() {
  this->service_init();
  this->commands_init();
  this->keymap_init();

  this->_cmdline = CmdLine::create();
  this->_cmdline->show();

  return this->exec();
}

bool App::notify(QObject *o, QEvent *e) {
  switch (e->type()) {
  case QEvent::KeyPress:
    this->_cur_key_ev = reinterpret_cast<QKeyEvent*>(e);
    if (this->shortcut_srv().disptach(reinterpret_cast<QKeyEvent *>(e))) {
      return true;
    }
    break;
  case QEvent::KeyRelease:
    this->_cur_key_ev = std::nullopt;
    break;
  default:
    break;
  }

  return QApplication::notify(o, e);
}

void App::service_init() {
  _command_srv = CommandService::create();
  _shortcut_srv = ShortcutService::create();
  _complete_srv = CompleteService::create();
}

void App::commands_init() {
  this->command_srv().add("exit", std::bind(QApplication::exit, 0));
}

void App::keymap_init() {
  this->shortcut_srv().map(
      KeyMap::make("global")->add("Ctrl+x, Ctrl+c", "exit"));
}

} // namespace my
