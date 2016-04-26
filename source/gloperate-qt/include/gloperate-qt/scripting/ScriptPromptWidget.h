#pragma once

#include <QTextEdit>

#include <gloperate-qt/gloperate-qt_api.h>


class QCompleter;
class QSyntaxHighlighter;


namespace gloperate_qt
{

/** \brief TextEdit widget behaving like a unix shell / windows command prompt. 
*
* Passed commad-lines will be emitted through the evaluate signal. The command 
* prompt shows a prefix for each command-line that is entered. All processed 
* commands are read only, leaving only the last command-line editable. 
* The cmd-prompt allows copy and pasting of text, multi-line texts or uris,
* supports undo-redo for the current command-line, and allows toggling between
* all previous processed line-commands.
* The command prompt can be clear-screened by pressing [Ctrl]+[L].
*/
class GLOPERATE_QT_API ScriptPromptWidget : public QTextEdit
{
    Q_OBJECT

public:
    ScriptPromptWidget(QWidget * parent = 0);
    virtual ~ScriptPromptWidget();

    void setMultiLinePaste(bool enable);
    bool multiLinePaste() const;

    /*
    * Note: widget takes ownership of the highlighter and deletes it on 
    * varrying reassign or in dtor.
    */
    void setSyntaxHighlighter(QSyntaxHighlighter * syntaxHighlighter);
    QSyntaxHighlighter * syntaxHighlighter();

    /*
    * Note: widget takes ownership of the completer and deletes it on
    * varrying reassign or in dtor.
    */
    void setCompleter(QCompleter * completer);
    QCompleter * completer();

    void setUrisPasteDelimiter(const QString & delimiter);
    const QString & urisPasteDelimiter() const;

    void setUrisPasteQuotMarks(bool enable);
    bool urisPasteQuotMarks() const;

signals:
    void evaluate(const QString & command);

public slots:
    void print(const QString & message);

protected:
    void process();
    /**
    * Clear Screen (currently accessible only via [Ctrl]+[L]).
    * While clearing, the current row remains unchanged.
    */
    void cls();

    int row() const;
    int column() const;
    int anchor() const;

    QString wordUnderCursor() const;

    void setPosition(int position, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor);
    int position() const;

    void resetUndoRedo();

protected:
    virtual void keyPressEvent(QKeyEvent * e);
    void completerKeyPressEvent(QKeyEvent * e);

    /**
    * Handle text pasting: If not in readOnly mode (text cursor is in last row),
    * the clipboard text will be inserted at the text cursor. If clipboard text
    * contains multiple lines, each line is processed as individual command.
    * If source containts a uri, it is passed as file name (comma-separated if
    * multiple uris are provided).
    */
    virtual void insertFromMimeData(const QMimeData * source);

    void insertUrlsFromMimeData(const QMimeData * source);
    void insertTextFromMimeData(const QMimeData * source);

    void updateLastInHistory();
    void setCommandFromHistory();

protected slots:
    void onCursorPositionChanged();

    /*
    * Used for QCompleter (activated signal)
    */
    void complete(const QString & text);

protected:
    int m_rows;
    int m_lastRowPos;

    bool m_multiLinePaste;

    QCompleter * m_completer;
    QSyntaxHighlighter * m_syntaxHighlighter;

    QString m_urisPasteDelimiter;
    bool m_urisPasteQuotMarks;

    QStringList m_history;
    QStringList::const_iterator m_historyIt;
};

} // namespace gloperate_qt
