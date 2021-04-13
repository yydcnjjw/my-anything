#include <app.hpp>

#include <QKeyEvent>
#include <boost/di.hpp>
#include <boost/di/extension/policies/types_dumper.hpp>
#include <service/complete_service.hpp>
#include <ui/cmdline.hpp>

namespace {
using namespace my;
struct Commands {
  static auto constexpr kFindFile{"find-file"};
  static auto constexpr kExecuteCommand{"execute-command"};
};

namespace di = boost::di;

auto di_configuration(argc_t argc, argv_t argv) {
  return di::make_injector<di::extension::types_dumper>(
      // di::bind<Args>.to(Args{argc, argv}),
      di::bind<App>.to<App>().in(di::singleton),
      di::bind<CommandService>.to<CommandService>().in(di::singleton),
      di::bind<CompleteService>.to<CompleteService>().in(di::singleton),
      di::bind<ShortcutService>.to<ShortcutService>().in(di::singleton),
      di::bind<CmdLine>.to<CmdLine>().in(di::singleton));
};

std::shared_ptr<Args> app_args;
} // namespace

namespace my {

int App::run(argc_t argc, argv_t argv) {
  app_args = std::make_shared<Args>(argc, argv);

  spdlog::set_level(spdlog::level::debug);

  auto injector = di_configuration(argc, argv);
  auto &app = injector.create<App &>();

  injector.create<CmdLine &>().show();

  SPDLOG_DEBUG("app exec...");

  return app.exec();
}

App::App(CommandService &command_srv, ShortcutService &shortcut_srv,
         CompleteService &complete_srv)
    : QApplication(app_args->argc, app_args->argv),
      _shortcut_srv(shortcut_srv) {

  this->commands_init(command_srv, complete_srv);
  this->keymap_init(shortcut_srv);
}

bool App::notify(QObject *o, QEvent *e) {
  switch (e->type()) {
  case QEvent::KeyPress: {
    auto key_ev = reinterpret_cast<QKeyEvent *>(e);
    this->_cur_key_ev = key_ev;
    if (this->_shortcut_srv.disptach(key_ev)) {
      return true;
    }
    break;
  }
  case QEvent::KeyRelease:
    this->_cur_key_ev = std::nullopt;
    break;
  default:
    break;
  }

  return QApplication::notify(o, e);
}

void App::commands_init(CommandService &command_srv,
                        CompleteService &complete_srv) {
  command_srv.add("exit", std::bind(QApplication::exit, 0));
  command_srv.add(Commands::kFindFile, [this, &complete_srv]() {
    auto file_complete = complete_srv.get_complete_backends("file").value();
    complete_srv
        .read_input("",
                    [file_complete](std::string const &input) {
                      return file_complete->complete(input);
                    })
        .subscribe(
            [](std::string const &v) { SPDLOG_DEBUG("find file: {}", v); });
  });
  command_srv.add(Commands::kExecuteCommand, [this, &command_srv,
                                              &complete_srv]() {
    std::vector<std::string> candidates;
    std::ranges::copy(command_srv.commands() | std::views::keys,
                      std::back_inserter(candidates));
    complete_srv.read_input("", candidates).subscribe([](std::string const &v) {
      SPDLOG_DEBUG("execute command: {}", v);
    });
  });
}

void App::keymap_init(ShortcutService &srv) {
  srv.map(srv.make_keymap("global")
              ->add("Ctrl+x, Ctrl+c", "exit")
              ->add("Ctrl+x, Ctrl+f", Commands::kFindFile)
              ->add("Alt+x", Commands::kExecuteCommand));
}

} // namespace my
