
#include <gloperate-qt/scripting/ScriptPromptWidget.h>

#include <cassert>

#include <QCompleter>
#include <QKeyEvent>
#include <QMimeData>
#include <QSyntaxHighlighter>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QAbstractItemView>
#include <QScrollBar>


namespace
{
    const QString ROW_PREFIX("> ");
    const int ROW_PREFIX_LENGTH(ROW_PREFIX.length());

    const Qt::TextInteractionFlags READ_INTERACTION_FLAGS = 
        Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard;

    const Qt::TextInteractionFlags WRITE_INTERACTION_FLAGS =
        Qt::TextEditorInteraction;
}


namespace gloperate_qt
{


ScriptPromptWidget::ScriptPromptWidget(QWidget * parent)
: QTextEdit(parent)
, m_rows(0)
, m_lastRowPos(ROW_PREFIX_LENGTH)
, m_multiLinePaste(true)
, m_completer(nullptr)
, m_syntaxHighlighter(nullptr)
, m_urisPasteDelimiter(", ")
, m_urisPasteQuotMarks(true)
{
    setFont(QFont("Consolas, Courier New"));
    connect(this, &QTextEdit::cursorPositionChanged, this, &ScriptPromptWidget::onCursorPositionChanged);

    append(ROW_PREFIX);
    cls();
}

ScriptPromptWidget::~ScriptPromptWidget()
{
}

bool ScriptPromptWidget::multiLinePaste() const
{
    return m_multiLinePaste;
}

void ScriptPromptWidget::setMultiLinePaste(const bool enable)
{
    m_multiLinePaste = enable;
}

const QSyntaxHighlighter * ScriptPromptWidget::syntaxHighlighter() const
{
    return m_syntaxHighlighter.get();
}

QSyntaxHighlighter * ScriptPromptWidget::syntaxHighlighter()
{
    return m_syntaxHighlighter.get();
}

void ScriptPromptWidget::setSyntaxHighlighter(std::unique_ptr<QSyntaxHighlighter> && syntaxHighlighter)
{
    m_syntaxHighlighter = std::move(syntaxHighlighter);

    if (m_syntaxHighlighter)
        m_syntaxHighlighter->setDocument(document());
}

const QCompleter * ScriptPromptWidget::completer() const
{
    return m_completer.get();
}

QCompleter * ScriptPromptWidget::completer()
{
    return m_completer.get();
}

void ScriptPromptWidget::setCompleter(std::unique_ptr<QCompleter> && completer)
{
    m_completer = std::move(completer);

    if (m_completer)
    {
        connect(m_completer.get(), SIGNAL(activated(QString)), this, SLOT(complete(QString)));

        m_completer->setWidget(this);
        m_completer->popup()->setFont(font());
    }
}

const QString & ScriptPromptWidget::urisPasteDelimiter() const
{
    return m_urisPasteDelimiter;
}

void ScriptPromptWidget::setUrisPasteDelimiter(const QString & delimiter)
{
    m_urisPasteDelimiter = delimiter;
}

bool ScriptPromptWidget::urisPasteQuotMarks() const
{
    return m_urisPasteQuotMarks;
}

void ScriptPromptWidget::setUrisPasteQuotMarks(bool enable)
{
    m_urisPasteQuotMarks = enable;
}

void ScriptPromptWidget::print(const QString & message)
{
    const int pos = position();

    moveCursor(QTextCursor::StartOfLine);
    textCursor().insertText(message + "\n");

    const int offset(message.length() + 1);
    m_lastRowPos += offset;
    setPosition(pos + offset);
}

void ScriptPromptWidget::process()
{
    const QString command = toPlainText().mid(m_lastRowPos);

    blockSignals(true);
    moveCursor(QTextCursor::MoveOperation::End);
    blockSignals(false);

    updateLastInHistory();
    m_history.append(""); // add new slot for upcomming command

    if (isReadOnly())
        setReadOnly(false);

    blockSignals(true);
    moveCursor(QTextCursor::MoveOperation::End);
    textCursor().insertText("\n" + ROW_PREFIX);
    blockSignals(false);

    emit evaluate(command); // might trigger calls to print slot

    ensureCursorVisible();

    resetUndoRedo();

    m_lastRowPos = position();
    m_rows = row();

    updateLastInHistory();
}

void ScriptPromptWidget::cls()
{
    const int newPosition = qMax(ROW_PREFIX_LENGTH, position() - m_lastRowPos + ROW_PREFIX_LENGTH);

    setPlainText(toPlainText().mid(m_lastRowPos - ROW_PREFIX_LENGTH));

    m_lastRowPos = ROW_PREFIX_LENGTH;
    m_rows = 1;

    setPosition(newPosition);
}

int ScriptPromptWidget::row() const
{
    return toPlainText().left(textCursor().position()).count("\n") + 1;
}

int ScriptPromptWidget::column() const
{
    return textCursor().columnNumber();
}

int ScriptPromptWidget::anchor() const
{
    return textCursor().anchor();
}

QString ScriptPromptWidget::wordUnderCursor() const
{
    QTextCursor tc(textCursor());

    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

int ScriptPromptWidget::position() const
{
    return textCursor().position();
}

void ScriptPromptWidget::setPosition(
    const int position
  , const QTextCursor::MoveMode mode)
{
    QTextCursor tc(textCursor());
    tc.setPosition(position, mode);

    setTextCursor(tc);
}

void ScriptPromptWidget::resetUndoRedo()
{
    setUndoRedoEnabled(false);
    setUndoRedoEnabled(true);
}

void ScriptPromptWidget::keyPressEvent(QKeyEvent * e)
{
    if (m_completer && m_completer->popup()->isVisible())
    {
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return;
        default:
            break;
        }
    }

    if(e->key() == Qt::Key_L && e->modifiers() & Qt::KeyboardModifier::ControlModifier)
        return cls();

    if (isReadOnly() && e->key() != Qt::Key_Enter && e->key() != Qt::Key_Return)
        return QTextEdit::keyPressEvent(e);

    switch (e->key())
    {
    case Qt::Key_Backspace:
        if (column() > ROW_PREFIX_LENGTH
         || (anchor() > position() && column() > ROW_PREFIX_LENGTH - 1))
            QTextEdit::keyPressEvent(e);
        break;

    case Qt::Key_Left:
        if (row() < m_rows || column() > ROW_PREFIX_LENGTH)
            QTextEdit::keyPressEvent(e);
        break;

    case Qt::Key_Home:
        if (row() < m_rows)
            QTextEdit::keyPressEvent(e);
        else
            setPosition(m_lastRowPos, e->modifiers().testFlag(Qt::KeyboardModifier::ShiftModifier) ? QTextCursor::KeepAnchor : QTextCursor::MoveAnchor);
        break;

    case Qt::Key_Up:
        if (e->modifiers().testFlag(Qt::NoModifier) && row() == m_rows
            && m_historyIt != m_history.begin())
        {
            --m_historyIt;
            setCommandFromHistory();
        }
        break;

    case Qt::Key_Down:
        if (e->modifiers().testFlag(Qt::NoModifier) && row() == m_rows)
        {
            ++m_historyIt;
            setCommandFromHistory();
        }
        break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
        process();
        break;

    default:
        QTextEdit::keyPressEvent(e);
        updateLastInHistory();
        break;
    }

    if (m_completer)
        completerKeyPressEvent(e);
}

void ScriptPromptWidget::completerKeyPressEvent(QKeyEvent * e)
{
    assert(m_completer);

    static const QString naw("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // not a word.

    const QString prefix(wordUnderCursor());

    if (e->text().isEmpty() || prefix.length() < 1 || naw.contains(e->text()))
        return m_completer->popup()->hide();

    if (prefix != m_completer->completionPrefix())
    {
        m_completer->setCompletionPrefix(prefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }

    QRect cr(cursorRect());
    cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
        + m_completer->popup()->verticalScrollBar()->sizeHint().width());

    m_completer->complete(cr);
}

void ScriptPromptWidget::insertFromMimeData(const QMimeData * source)
{
    assert(source);

    if (source->hasUrls())
        return insertUrlsFromMimeData(source);
    if (source->hasText())
        return insertTextFromMimeData(source);
}

void ScriptPromptWidget::insertUrlsFromMimeData(const QMimeData * source)
{
    assert(source);

    QString mime;
    QString qm = m_urisPasteQuotMarks ? "\"" : "";

    for (const QUrl & url : source->urls())
    {
        mime.append(qm + url.toString().replace("///", "//").replace("file://", "") + qm);
        mime.append(m_urisPasteDelimiter);
    }
    mime.remove(mime.length() - m_urisPasteDelimiter.length(), m_urisPasteDelimiter.length());

    textCursor().insertText(mime);
}

void ScriptPromptWidget::insertTextFromMimeData(const QMimeData * source)
{
    assert(source);

    if (m_multiLinePaste)
    {
        textCursor().insertText(source->text());
        process();
    }
    else
    {
        QStringList commands = source->text().split("\n");
        for (QString & command : commands)
        {
            // Note: the first command gets appended to the current line
            textCursor().insertText(command);
            if (commands.size() > 1)
                process();
        }
    }
}

void ScriptPromptWidget::updateLastInHistory()
{
    if (!m_history.isEmpty())
        m_history.removeLast();

    m_history.append(toPlainText().mid(m_lastRowPos));

    m_historyIt = m_history.constEnd();
    --m_historyIt;
}

void ScriptPromptWidget::setCommandFromHistory()
{
    if (m_history.constEnd() == m_historyIt)
        --m_historyIt;

    QTextCursor tc(textCursor());
    tc.setPosition(m_lastRowPos);
    tc.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    tc.removeSelectedText();
    tc.insertText(*m_historyIt);
    tc.movePosition(QTextCursor::End);
    setTextCursor(tc);
}

void ScriptPromptWidget::onCursorPositionChanged()
{
    setReadOnly(position() < m_lastRowPos || anchor() < m_lastRowPos);
    setTextInteractionFlags(isReadOnly() ? READ_INTERACTION_FLAGS : WRITE_INTERACTION_FLAGS);
}

void ScriptPromptWidget::complete(const QString & text)
{
    QTextCursor tc(textCursor());

    tc.select(QTextCursor::WordUnderCursor);
    tc.removeSelectedText();

    tc.insertText(text);
    tc.movePosition(QTextCursor::EndOfWord);

    setTextCursor(tc);
    updateLastInHistory();
}


} // namespace gloperate_qt
