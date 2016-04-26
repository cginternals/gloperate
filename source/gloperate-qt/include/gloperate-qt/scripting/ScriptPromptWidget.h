
#pragma once


#include <QTextEdit>

#include <gloperate-qt/gloperate-qt_api.h>


class QCompleter;
class QSyntaxHighlighter;


namespace gloperate_qt
{


/**
*  @brief
*    TextEdit widget behaving like a unix shell / windows command prompt
*
*    Passed commad-lines will be emitted through the evaluate signal. The command 
*    prompt shows a prefix for each command-line that is entered. All processed 
*    commands are read only, leaving only the last command-line editable. 
*    The cmd-prompt allows copy and pasting of text, multi-line texts or uris,
*    supports undo-redo for the current command-line, and allows toggling between
*    all previous processed line-commands.
*    The command prompt can be clear-screened by pressing [Ctrl]+[L].
*/
class GLOPERATE_QT_API ScriptPromptWidget : public QTextEdit
{
Q_OBJECT


signals:
    /**
    *  @brief
    *    Emitted when a command has been entered on the script prompt
    *
    *  @param[in] command
    *    Input entered by the user
    */
    void evaluate(const QString & command);


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent widget (can be null)
    */
    ScriptPromptWidget(QWidget * parent = 0);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptPromptWidget();

    /**
    *  @brief
    *    Check if multi-line text can be pasted
    *
    *  @return
    *    'true' if active, else 'false'
    */
    bool multiLinePaste() const;

    /**
    *  @brief
    *    Set if multi-line text can be pasted
    *
    *  @param[in] enable
    *    'true' if active, else 'false'
    */
    void setMultiLinePaste(bool enable);

    /**
    *  @brief
    *    Get assigned syntax highlighter
    *
    *  @return
    *    Pointer to syntax highlighter (can be null)
    */
    QSyntaxHighlighter * syntaxHighlighter() const;

    /**
    *  @brief
    *    Assign syntax highlighter
    *
    *  @param[in] syntaxHighlighter
    *    Pointer to syntax highlighter (can be null)
    *
    *  @remarks
    *    The widget takes ownership of the highlighter and deletes it on 
    *    varrying reassign or in dtor.
    */
    void setSyntaxHighlighter(QSyntaxHighlighter * syntaxHighlighter);

    /**
    *  @brief
    *    Get assigned text completer
    *
    *  @return
    *    Pointer to text completer (can be null)
    */
    QCompleter * completer() const;

    /**
    *  @brief
    *    Assign text completer
    *
    *  @param[in] completer
    *    Pointer to text completer (can be null)
    *
    *  @remarks
    *    The widget takes ownership of the completer and deletes it on 
    *    varrying reassign or in dtor.
    */
    void setCompleter(QCompleter * completer);

    /**
    *  @brief
    *    Get delimiter that is inserted when pasting URIs
    *
    *  @return
    *    Delimiter (default: ', ')
    */
    const QString & urisPasteDelimiter() const;

    /**
    *  @brief
    *    Set delimiter that is inserted when pasting URIs
    *
    *  @param[in] delimiter
    *    Delimiter (default: ', ')
    */
    void setUrisPasteDelimiter(const QString & delimiter);

    /**
    *  @brief
    *    Check if quotation marks are inserted when pasting URIs
    *
    *  @return
    *    'true' for using quoation marks, else 'false'
    */
    bool urisPasteQuotMarks() const;

    /**
    *  @brief
    *    Set if quotation marks are inserted when pasting URIs
    *
    *  @param[in] enable
    *    'true' for using quoation marks, else 'false'
    */
    void setUrisPasteQuotMarks(bool enable);


public slots:
    /**
    *  @brief
    *    Print a line of text to the script prompt
    *
    *  @param[in] message
    *    Message
    */
    void print(const QString & message);


protected:
    /**
    *  @brief
    *    Process the current input text and emit it as a command
    */
    void process();

    /**
    *  @brief
    *    Clear script prompt
    *
    *  @notes
    *    - Currently accessible only via [Ctrl]+[L]
    *    - While clearing, the current row remains unchanged
    */
    void cls();

    /**
    *  @brief
    *    Get current row
    *
    *  @return
    *    Row number
    */
    int row() const;

    /**
    *  @brief
    *    Get current column
    *
    *  @return
    *    Column number
    */
    int column() const;

    /**
    *  @brief
    *    Get current anchor
    *
    *  @return
    *    Anchor
    */
    int anchor() const;

    /**
    *  @brief
    *    Get words that is currently under the cursor
    *
    *  @return
    *    Word
    */
    QString wordUnderCursor() const;

    /**
    *  @brief
    *    Get current cursor position in the text
    *
    *  @return
    *    Cursor position in the text
    */
    int position() const;

    /**
    *  @brief
    *    Set cursor position in the text
    *
    *  @param[in] position
    *    Cursor position in the text
    *  @param[in] mode
    *    Move mode
    */
    void setPosition(int position, QTextCursor::MoveMode mode = QTextCursor::MoveAnchor);

    /**
    *  @brief
    *    Reset undo/redo stack
    */
    void resetUndoRedo();

    /**
    *  @brief
    *    Handle key events
    *
    *  @param[in] e
    *    Keyboard event
    */
    virtual void keyPressEvent(QKeyEvent * e);

    /**
    *  @brief
    *    Update completer when handling key events
    *
    *  @param[in] e
    *    Keyboard event
    */
    void completerKeyPressEvent(QKeyEvent * e);

    /**
    *  @brief
    *    Handle clipboard
    *
    *  @param[in] source
    *    MIME data source
    *
    *  @remarks
    *    If not in readOnly mode (text cursor is in last row), the clipboard
    *    text will be inserted at the text cursor. If clipboard text contains
    *    multiple lines, each line is processed as individual command. If
    *    source contains a URI, it is passed as file name (comma-separated,
    *    if multiple URIs are provided).
    */
    virtual void insertFromMimeData(const QMimeData * source);

    /**
    *  @brief
    *    Handle inserting URLs
    *
    *  @param[in] source
    *    MIME data source
    */
    void insertUrlsFromMimeData(const QMimeData * source);

    /**
    *  @brief
    *    Handle inserting text
    *
    *  @param[in] source
    *    MIME data source
    */
    void insertTextFromMimeData(const QMimeData * source);

    /**
    *  @brief
    *    Update command history
    */
    void updateLastInHistory();

    /**
    *  @brief
    *    Set command from last in history
    */
    void setCommandFromHistory();


protected slots:
    /**
    *  @brief
    *    Called when cursor position has changed
    */
    void onCursorPositionChanged();

    /**
    *  @brief
    *    Used for QCompleter (activated signal)
    *
    *  @param[in] text
    *    Text that is completed
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
