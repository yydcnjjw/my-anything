#pragma once

#include <stack>

#include <QLineEdit>
#include <QListWidget>

#include <app.hpp>

namespace my {

class CmdLine : public QWidget {
  Q_OBJECT

public:
  UNIQUE_CLS(CmdLine)
  CmdLine(App &app, CommandService &command_srv, ShortcutService &shortcut_srv,
          CompleteService &complete_srv);
  ~CmdLine();

  void next_line() {
    auto row{this->_list_view->currentRow()};

    if (row == this->_list_view->count()) {
      row = 0;
    } else {
      ++row;
    }

    this->_list_view->setCurrentRow(row);
  }

  void previous_line() {
    auto row{this->_list_view->currentRow()};

    if (row == 0) {
      row = this->_list_view->count() - 1;
    } else {
      --row;
    }

    this->_list_view->setCurrentRow(row);
  }

  void set_text(std::string const &text) {
    this->_line_edit->setText(text.c_str());
  }

  std::optional<QListWidgetItem *> selected_item() {
    auto list = this->_list_view->selectedItems();
    if (list.empty()) {
      return std::nullopt;
    } else {
      return list.first();
    }
  }

  template <typename Iter> void set_complete_list(Iter &&candidates) {
    this->_list_view->clear();
    for (auto &complete_item : candidates) {
      auto item =
          new QListWidgetItem(complete_item->desc().c_str(), this->_list_view);
      item->setData(Qt::UserRole, QVariant::fromValue(complete_item));
      this->_list_view->addItem(item);
    }
  }

  rx::observable<std::string> text_changed() {
    return this->_text_changed.get_observable();
  }

  rx::observable<int> item_selected() {
    return this->_item_selected.get_observable();
  }

private:
  App &_app;
  QLineEdit *_line_edit;
  QListWidget *_list_view;

  bool _text_mark{false};

  rx::subjects::subject<std::string> _text_changed;
  rx::subjects::subject<int> _item_selected;

  void ui_init();
  void event_init();

  void commands_init(CommandService &srv);
  void keymap_init(ShortcutService &srv);
};

} // namespace my
