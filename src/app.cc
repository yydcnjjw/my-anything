#include "app.hpp"

#include <QKeyEvent>
#include <boost/di.hpp>
#include <boost/di/extension/policies/types_dumper.hpp>
#include <boost/di/extension/scopes/shared.hpp>
#include <document/document.hpp>
#include <document/org/org.hpp>
#include <service/complete_service.hpp>
#include <ui/cmdline.hpp>

namespace {
using namespace my;
struct Commands {
  static auto constexpr kFindFile{"find-file"};
  static auto constexpr kExecuteCommand{"execute-command"};
};

namespace di = boost::di;

auto document_configuration() {
  return di::make_injector<di::extension::types_dumper>(
      di::bind<NNetLanguageIdentifier>.to<>(
          std::make_shared<NNetLanguageIdentifier>(0, 4096)),
      di::bind<DocumentBuilder>().to<DocumentBuilder>().in(
          di::extension::shared));
}

auto service_configuration() {
  return di::make_injector<di::extension::types_dumper>(
      di::bind<CommandService>.to<CommandService>().in(di::extension::shared),
      di::bind<CompleteService>.to<CompleteService>().in(di::extension::shared),
      di::bind<ShortcutService>.to<ShortcutService>().in(
          di::extension::shared));
}

auto app_configuration(argc_t argc, argv_t argv) {
  return di::make_injector<di::extension::types_dumper>(
      di::bind<Args>.to(Args{argc, argv}),
      di::bind<App>.to<App>().in(di::extension::shared),
      service_configuration(), document_configuration(),
      di::bind<CmdLine>.to<CmdLine>().in(di::extension::shared));
};

} // namespace

namespace my {

int App::run(argc_t argc, argv_t argv) {
  spdlog::set_level(spdlog::level::debug);
  auto inject = app_configuration(argc, argv);

  return 0;
  // auto &app = inject.create<App &>();

  // inject.create<CmdLine &>().show();

  // return app.exec();
}

App::App(Args args, CommandService &command_srv, ShortcutService &shortcut_srv,
         CompleteService &complete_srv)
    : QApplication(args.argc, args.argv), _shortcut_srv(shortcut_srv) {
  this->commands_init(command_srv, complete_srv);
  this->keymap_init(shortcut_srv);
}

bool App::notify(QObject *o, QEvent *e) {
  switch (e->type()) {
  case QEvent::KeyPress: {
    auto key_ev = reinterpret_cast<QKeyEvent *>(e);
    this->_cur_key_ev.reset(key_ev->clone());
    if (this->_shortcut_srv.disptach(key_ev)) {
      return true;
    }
    break;
  }
  case QEvent::KeyRelease:
    this->_cur_key_ev.reset();
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
