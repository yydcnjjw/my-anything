#include "cmdline.hpp"

#include <sstream>

#include <QAction>
#include <QScreen>
#include <QVBoxLayout>

namespace {
using namespace my;
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

class CmdLineInteractiveBackend : public CompleteService::InteractiveBackend {
public:
  SHARED_CLS(CmdLineInteractiveBackend)
  CmdLineInteractiveBackend(CmdLine &cmdline) : _cmdline(cmdline) {}

  void update_items(CompleteService::CompleteCtx::ptr_t const &ctx) override {
    _cmdline.set_text(ctx->input());
    _cmdline.set_complete_list(ctx->candidates());
  }

  void clear_items() override {
    _cmdline.set_text("");
    _cmdline.set_complete_list(std::ranges::empty_view<CompleteItem::ptr_t>{});
  }

  rx::observable<std::string> text_changed() override {
    return _cmdline.text_changed();
  }

  rx::observable<int> item_selected() override {
    return _cmdline.item_selected();
  }

private:
  CmdLine &_cmdline;
};

} // namespace

namespace my {

CmdLine::CmdLine(App &app, CommandService &command_srv,
                 ShortcutService &shortcut_srv, CompleteService &complete_srv)
    : QWidget(nullptr), _app(app) {
  this->ui_init();
  this->event_init();
  this->commands_init(command_srv);
  this->keymap_init(shortcut_srv);

  complete_srv.set_interactive_backend(CmdLineInteractiveBackend::make(*this));
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

void CmdLine::commands_init(CommandService &srv) {
  srv.add(Commands::kBeginningOfLine,
          std::bind(&QLineEdit::home, this->_line_edit,
                    std::cref(this->_text_mark)))
      .add(Commands::kEndOfLine, std::bind(&QLineEdit::end, this->_line_edit,
                                           std::cref(this->_text_mark)))
      .add(Commands::kBackspace,
           std::bind(&QLineEdit::backspace, this->_line_edit))
      .add(Commands::kInsertChar,
           [this]() {
             auto &ev = this->_app.cur_key_ev();
             if (!ev) {
               return;
             }
             this->_line_edit->insert((*ev)->text());
           })
      .add(Commands::kNextLine, std::bind(&CmdLine::next_line, this))
      .add(Commands::kPreviousLine, std::bind(&CmdLine::previous_line, this))
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

void CmdLine::keymap_init(ShortcutService &srv) {
  auto keymap = srv.make_keymap("cmdline")
                    ->add("Ctrl+a", Commands::kBeginningOfLine)
                    ->add("Ctrl+e", Commands::kEndOfLine)
                    ->add(QKeySequence(Qt::Key_Backspace), Commands::kBackspace)
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

  srv.map(keymap);
}

void CmdLine::event_init() {
  QObject::connect(
      this->_line_edit, &QLineEdit::textChanged, [this](QString const &text) {
        this->_text_changed.get_subscriber().on_next(text.toStdString());
      });
  QObject::connect(this->_list_view, &QListWidget::currentRowChanged,
                   [this](int cur_row) {
                     this->_item_selected.get_subscriber().on_next(cur_row);
                   });
}

} // namespace my
