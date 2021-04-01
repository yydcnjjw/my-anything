#include "cmdline.hpp"

#include <sstream>

#include <QAction>
#include <QScreen>
#include <QVBoxLayout>

#include <app.hpp>

namespace {
struct Commands {
  static auto constexpr kBeginningOfLine{"_beginning-of-line"};
  static auto constexpr kEndOfLine{"_end-of-line"};
  static auto constexpr kBackspace{"_backspace"};
  static auto constexpr kEnter{"_enter"};
  static auto constexpr kInsertChar{"_insert-char"};
  static auto constexpr kNextLine{"_next-line"};
  static auto constexpr kPreviousLine{"_previous-line"};
  static auto constexpr kForwardChar{"_forward-char"};
  static auto constexpr kBackwardChar{"_backward-char"};
  static auto constexpr kForwardWord{"_forward-word"};
  static auto constexpr kBackwardWord{"_backward-word"};
  static auto constexpr kFrowardDelete{"_forward-delete"};
};
} // namespace

namespace my {

CmdLine::CmdLine(QWidget *parent) : QWidget(parent) {
  this->ui_init();
  this->event_init();
  this->commands_init();
  this->key_map_init();
}

CmdLine::~CmdLine() {}

void CmdLine::ui_init() {
  setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

  {
    auto screen_size = screen()->availableSize();
    setMinimumSize(screen_size.width() * 0.6, 48);
    move((screen_size.width() - width()) / 2, 10);
  }

  QFont font;
  {
    font.setFamily("Hack");
    font.setPixelSize(36);
    this->setFont(font);
  }

  auto layout{new QVBoxLayout(this)};
  layout->setContentsMargins(0, 0, 0, 10);

  {
    _line_edit = new QLineEdit();
    _line_edit->setAlignment(Qt::AlignCenter);
    _line_edit->setPlaceholderText("Enter: ");
    _line_edit->setFixedHeight(48);
    layout->addWidget(_line_edit);
  }

  {
    _list_view = new QListWidget();
    font.setPixelSize(28);
    _list_view->setFont(font);
    layout->addWidget(_list_view);
  }

  this->setLayout(layout);
}

void CmdLine::commands_init() {
  auto &srv = App::get().command_srv();
  srv.add(Commands::kBeginningOfLine,
          std::bind(&QLineEdit::home, this->_line_edit,
                    std::cref(this->_text_mark)))
      .add(Commands::kEndOfLine, std::bind(&QLineEdit::end, this->_line_edit,
                                           std::cref(this->_text_mark)))
      .add(Commands::kBackspace,
           std::bind(&QLineEdit::backspace, this->_line_edit))
      .add(Commands::kEnter,
           [this]() {
             auto list = this->_list_view->selectedItems();
             if (!list.empty()) {
               list.at(0)
                   ->data(Qt::UserRole)
                   .value<CompleteItem::ptr_t>()
                   ->exec();
             }
           })
      .add(Commands::kInsertChar,
           [this]() {
             auto &ev = App::get().cur_key_ev();
             if (!ev) {
               return;
             }
             this->_line_edit->insert((*ev)->text());
           })
      .add(Commands::kNextLine,
           [this]() {
             this->_list_view->setCurrentRow(this->_list_view->currentRow() +
                                             1);
           })
      .add(Commands::kPreviousLine,
           [this]() {
             this->_list_view->setCurrentRow(this->_list_view->currentRow() -
                                             1);
           })
      .add(Commands::kForwardChar,
           std::bind(&QLineEdit::cursorForward, this->_line_edit,
                     std::cref(this->_text_mark), 1))
      .add(Commands::kBackwardChar,
           std::bind(&QLineEdit::cursorBackward, this->_line_edit,
                     std::cref(this->_text_mark), 1))
      .add(Commands::kForwardWord,
           std::bind(&QLineEdit::cursorWordForward, this->_line_edit,
                     std::cref(this->_text_mark)))
      .add(Commands::kBackwardWord,
           std::bind(&QLineEdit::cursorWordBackward, this->_line_edit,
                     std::cref(this->_text_mark)))
      .add(Commands::kFrowardDelete,
           std::bind(&QLineEdit::del, this->_line_edit));
}

void CmdLine::key_map_init() {
  auto keymap = KeyMap::make("cmdline")
                    ->add("Ctrl+a", Commands::kBeginningOfLine)
                    ->add("Ctrl+e", Commands::kEndOfLine)
                    ->add(QKeySequence(Qt::Key_Backspace), Commands::kBackspace)
                    ->add(QKeySequence(Qt::Key_Return), Commands::kEnter)
                    ->add("Ctrl+n", Commands::kNextLine)
                    ->add("Ctrl+p", Commands::kPreviousLine)
                    ->add("Ctrl+f", Commands::kForwardChar)
                    ->add("Ctrl+b", Commands::kBackwardChar)
                    ->add("Alt+f", Commands::kForwardWord)
                    ->add("Alt+b", Commands::kBackwardWord)
                    ->add("Ctrl+d", Commands::kFrowardDelete);

  for (int key{Qt::Key_Space}; key <= Qt::Key_QuoteLeft; ++key) {
    keymap->add(QKeySequence(key), Commands::kInsertChar);
    keymap->add(QKeySequence(Qt::SHIFT | static_cast<Qt::Key>(key)),
                Commands::kInsertChar);
  }

  for (auto key : {Qt::Key_BraceLeft, Qt::Key_Bar, Qt::Key_BraceRight,
                   Qt::Key_AsciiTilde}) {
    keymap->add(QKeySequence(key), Commands::kInsertChar);
    keymap->add(QKeySequence(Qt::SHIFT | key), Commands::kInsertChar);
  }

  App::get().shortcut_srv().map(keymap);
}

void CmdLine::on_text_changed(QString const &text) {
  this->_list_view->clear();

  complete_list list;
  App::get().complete_srv().disptach(text.toStdString(), list);

  for (auto &complete_item : list) {
    auto item =
        new QListWidgetItem(complete_item->desc().c_str(), this->_list_view);
    item->setData(Qt::UserRole, QVariant::fromValue(complete_item));
    this->_list_view->addItem(item);
  }

  this->_list_view->setCurrentRow(0);
}

void CmdLine::event_init() {
  connect(this->_line_edit, &QLineEdit::textChanged, this,
          &CmdLine::on_text_changed);
}

} // namespace my
