#include <service/shortcut_service.hpp>

#include <app.hpp>

namespace my {

KeyMap::ptr_t KeyMap::add(std::string const &keyseq, std::string const &cmd) {
  return this->add(KeyBind::make(this->shared_from_this(),
                                 QKeySequence(keyseq.c_str()),
                                 _srv._command_srv.get_command(cmd).value()));
}

KeyMap::ptr_t KeyMap::add(QKeySequence const &keyseq, std::string const &cmd) {
  return this->add(KeyBind::make(this->shared_from_this(), keyseq,
                                 _srv._command_srv.get_command(cmd).value()));
}

} // namespace my
