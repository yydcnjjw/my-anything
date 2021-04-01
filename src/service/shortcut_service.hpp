#pragma once

#include <QKeyEvent>

#include <service/command_service.hpp>

namespace my {
class KeyMap;

class KeyBind : public std::enable_shared_from_this<KeyBind> {
public:
  SHARED_CLS(KeyBind)
  using keymap_ptr_t = std::shared_ptr<const KeyMap>;

  KeyBind(keymap_ptr_t const &keymap, QKeySequence const &,
          std::string const &cmd_name);
  KeyBind(keymap_ptr_t const &keymap, std::string const &keyseq_str,
          std::string const &cmd_name);
  KeyBind(keymap_ptr_t const &keymap, QKeySequence const &keyseq,
          Command::ptr_t cmd)
      : _keymap(keymap), _keyseq(keyseq), _cmd(cmd) {}

  ~KeyBind() { this->cmd()->remove_keybind(this->shared_from_this()); }

  QKeySequence const &keyseq() const { return this->_keyseq; }

  std::string keyseq_str() const {
    return this->keyseq().toString().toStdString();
  }

  Command::ptr_t const &cmd() const { return this->_cmd; }

  template <typename... Args> void exec(Args &&...args) {
    if (this->cmd()) {
      this->cmd()->exec(std::forward<Args>(args)...);
    }
  }

  keymap_ptr_t const &keymap() { return this->_keymap; }

private:
  std::shared_ptr<const KeyMap> _keymap;
  QKeySequence _keyseq;
  Command::ptr_t _cmd;

  friend class Command;
  void cmd(Command::ptr_t const &cmd) { this->_cmd = cmd; }
  friend class KeyMap;

  void map_to_cmd() { this->_cmd->add_keybind(this->shared_from_this()); }
};

using keybind_container_t = std::unordered_map<std::string, KeyBind::ptr_t>;

class KeyMap : public std::enable_shared_from_this<KeyMap> {
public:
  SHARED_CLS(KeyMap)

  template <typename... Args> KeyMap(std::string const &name) : _name(name) {
  }

  std::string const &name() const { return this->_name; }
  keybind_container_t const &map() const { return this->_keybinds; }

  template <typename... Args,
            typename = std::enable_if_t<!std::conjunction_v<
                std::is_same<KeyBind::ptr_t, std::decay_t<Args>>...>>>
  ptr_t add(Args &&...args) {
    auto kb{
        KeyBind::make(this->shared_from_this(), std::forward<Args>(args)...)};
    kb->map_to_cmd();
    return this->add(kb);
  }

  ptr_t add(KeyBind::ptr_t const &keybind) {

    for (auto &item : this->_keybinds) {
      if (item.second->keyseq().matches(keybind->keyseq()) ==
          QKeySequence::PartialMatch) {
        throw std::runtime_error(
            (boost::format("Key sequence %s starts with non-prefix key %s") %
             keybind->keyseq_str() % item.second->keyseq_str())
                .str());
      }
    }

    this->_keybinds.insert_or_assign(keybind->keyseq_str(), keybind);
    return this->shared_from_this();
  }

private:
  std::string _name; // unique id
  keybind_container_t _keybinds;
};

class ShortcutService {
public:
  using keymap_container_t = std::unordered_map<std::string, KeyMap::ptr_t>;

  UNIQUE_CLS(ShortcutService)

  ShortcutService() = default;

  struct KeyBindCandidate {
    std::optional<KeyBind::ptr_t> exact_match;
    std::vector<KeyBind::ptr_t> partial_matches;
  };

  std::optional<KeyBind::ptr_t> exact_match(QKeySequence const &keyseq) const {

    auto v = this->_keymaps | std::views::values |
             std::views::filter([&keyseq](auto const &keymaps) {
               return keymaps->map().count(keyseq.toString().toStdString());
             }) |
             std::views::transform([&keyseq](auto const &keymaps) {
               return keymaps->map().at(keyseq.toString().toStdString());
             }) |
             std::views::take(1);

    if (v.begin() != v.end()) {
      return *v.begin();
    } else {
      return std::nullopt;
    }
  }

  KeyBindCandidate partial_match(QKeySequence const &keyseq) const {
    // return this->_keymaps | std::views::transform([](auto const &kv) {
    //          return kv.second->map() | std::views::values;
    //        }) |
    //        std::views::join |
    //        std::views::filter([&keyseq](auto const &keybind) {
    //          auto v = keybind->keyseq().matches(keyseq);
    //          return v == QKeySequence::ExactMatch ||
    //                 v == QKeySequence::PartialMatch;
    //          return true;
    //        });

    KeyBindCandidate candidate;

    for (auto &keymap : this->_keymaps | std::views::values) {

      for (auto &keybind : keymap->map() | std::views::values) {

        auto v = keyseq.matches(keybind->keyseq());

        if (v == QKeySequence::ExactMatch) {

          candidate.exact_match = keybind;

        } else if (v == QKeySequence::PartialMatch) {

          candidate.partial_matches.push_back(keybind);
        }
      }
    }

    return candidate;
  }

  bool disptach(QKeyEvent *e) {
    auto key{e->key()};

    // filter unknown, modifiers key, extra key
    switch (key) {
    case Qt::Key_unknown:
    case Qt::Key_Control:
    case Qt::Key_Shift:
    case Qt::Key_Alt:
    case Qt::Key_Meta:
    case Qt::Key_Super_L:
    case Qt::Key_Super_R:
    case Qt::Key_Hyper_L:
    case Qt::Key_Hyper_R:
      this->_cur_keyseq = {};
      return false;
    }

    QKeySequence keyseq(e->modifiers() | e->key());

    if (this->_cur_keyseq.isEmpty()) {
      this->_cur_keyseq = keyseq;
    } else {
      this->_cur_keyseq =
          QKeySequence::fromString((boost::format("%s, %s") %
                                    this->_cur_keyseq.toString().toStdString() %
                                    keyseq.toString().toStdString())
                                       .str()
                                       .c_str());
    }

    SPDLOG_DEBUG(this->_cur_keyseq.toString().toStdString());

    auto candidate = this->partial_match(this->_cur_keyseq);

    if (candidate.exact_match) {
      (*candidate.exact_match)->exec();
      this->_cur_keyseq = {};
    } else if (candidate.partial_matches.empty()) {
      this->_cur_keyseq = {};
    }

    return true;
  }

  void map(KeyMap::ptr_t keymap) {
    this->_keymaps.emplace(keymap->name(), keymap);
  }

  void unmap(std::string const &name) { this->_keymaps.erase(name); }

private:
  keymap_container_t _keymaps;
  QKeySequence _cur_keyseq;
};

} // namespace my
