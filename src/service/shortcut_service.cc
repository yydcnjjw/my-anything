#include <service/shortcut_service.hpp>

#include <app.hpp>

namespace my {

KeyBind::KeyBind(keymap_ptr_t const &keymap, QKeySequence const &keyseq,
                 std::string const &cmd_name)
    : KeyBind(keymap, keyseq, *App::get().command_srv().get_command(cmd_name)) {
}

KeyBind::KeyBind(keymap_ptr_t const &keymap, std::string const &keyseq_str,
                 std::string const &cmd_name)
    : KeyBind(keymap, QKeySequence(keyseq_str.c_str()),
              *App::get().command_srv().get_command(cmd_name)) {}

} // namespace my
