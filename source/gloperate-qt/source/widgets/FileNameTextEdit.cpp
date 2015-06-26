
#include <gloperate-qt/widgets/FileNameTextEdit.h>

#include <gloperate/ext-includes-begin.h>
#include <QCompleter>
#include <QKeyEvent>
#include <QFocusEvent>
#include <QAbstractItemView>
#include <QScrollBar>
#include <gloperate/ext-includes-end.h>


namespace gloperate_qt
{


FileNameTextEdit::FileNameTextEdit(QWidget *parent)
:   QPlainTextEdit(parent)
,   m_completer(0)
{
    setMaximumBlockCount(1);
}


FileNameTextEdit::~FileNameTextEdit()
{
    delete m_completer;
}


void FileNameTextEdit::setCompleter(QCompleter *completer)
{
    if (m_completer)
        QObject::disconnect(m_completer, 0, this, 0);

    m_completer = completer;

    if (!m_completer)
        return;

    m_completer->setWidget(this);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    connect(m_completer, static_cast<void(QCompleter::*)(const QString&)>(&QCompleter::activated),
        this, &FileNameTextEdit::insertCompletion);
}


QCompleter *FileNameTextEdit::completer() const
{
    return m_completer;
}

void FileNameTextEdit::insertCompletion(const QString& completion)
{
    if (m_completer->widget() != this)
        return;

    QTextCursor tc = textCursor();
    int extra = completion.length() - m_completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}


QString FileNameTextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();

    tc.select(QTextCursor::BlockUnderCursor);

    auto block = tc.selectedText();
    int i = block.lastIndexOf("<");
    int i2 = block.lastIndexOf(">");
    
    if (i < 0 || ( i < i2))
        return "";
    
    i = block.length() - i;

    tc.setPosition(tc.position());
    tc.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, i);

    tc.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, i);
    return tc.selectedText();
}


void FileNameTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_completer)
        m_completer->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}


void FileNameTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (m_completer && m_completer->popup()->isVisible())
    {
        // The following keys are forwarded by the completer to the widget
        switch (e->key()) 
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior

        default:
            break;
        }
    }
    else
    {
        switch (e->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            e->ignore();
            return; // let the completer do default behavior
        default:
            break;
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+E
    if (!m_completer || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrl = e->modifiers() & Qt::ControlModifier;
    if (!m_completer || (ctrl && e->key() != Qt::Key_Space && e->text().isEmpty()))
    {
        m_completer->popup()->hide();
        return;
    }

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!m_completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@$%^&*()_+{}|:\"?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 1
        || eow.contains(e->text().right(1)))) {
        m_completer->popup()->hide();
        return;
    }

    if (completionPrefix != m_completer->completionPrefix())
    {
        m_completer->setCompletionPrefix(completionPrefix);
        m_completer->popup()->setCurrentIndex(m_completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(m_completer->popup()->sizeHintForColumn(0)
        + m_completer->popup()->verticalScrollBar()->sizeHint().width());
    m_completer->complete(cr); // popup it up!
}


} // namespace gloperate_qt
