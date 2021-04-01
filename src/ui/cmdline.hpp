#pragma once

#include <QLineEdit>
#include <QListWidget>

#include <core/core.hpp>
#include <service/shortcut_service.hpp>

namespace my {

class CmdLine : public QWidget {
  Q_OBJECT

public:
  UNIQUE_CLS(CmdLine)
  CmdLine(QWidget *parent = nullptr);
  ~CmdLine();

  void set_text(std::string const &text) {
    this->_line_edit->setText(text.c_str());
  }

private:
  QLineEdit *_line_edit;
  QListWidget *_list_view;
  bool _text_mark{false};

  void ui_init();
  void event_init();

  void commands_init();
  void key_map_init();

  void on_text_changed(QString const &);
};
} // namespace my
