
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QPlainTextEdit>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class QCompleter;
class QKeyEvent;
class QFocusEvent;


namespace gloperate_qt
{


/**
*  @brief
*    Represent ppi or pixel per cm unit and value
*/
class GLOPERATE_QT_API FileNameTextEdit : public QPlainTextEdit
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    FileNameTextEdit(QWidget *parent = 0);

    /**
    *  @brief
    *    Destructor
    */
    ~FileNameTextEdit();

    /**
    *  @brief
    *    Setter of m_completer
    *
    *  @param[in] c
    *    A QCompleter, null_ptr is allowed
    */
    virtual void setCompleter(QCompleter *c);

    /**
    *  @brief
    *    Getter of m_completer
    *
    *  @return
    *    Pointer to m_completer
    */
    virtual QCompleter *completer() const;


protected:
    /**
    *  @brief
    *    Pop completer up if former wirtten text has completions
    *
    *  @param[in] e
    *    Keyevent, null_ptr is not allowed
    *
    *  @remarks
    *    Custom key handling for tab, backtab, enter, escape and return if
    *    completer is showen and for enter and return otherwise. Other keys
    *    are executed with their default behavior.
    */
    virtual void keyPressEvent(QKeyEvent *e);

    /**
    *  @brief
    *    Connect completer to widget if it gets focus
    *
    *  @param[in] e
    *    Focus event
    *
    *  @remarks
    *    Invoke default behavior after the connection is established.
    */
    virtual void focusInEvent(QFocusEvent *e);


private slots:
    virtual void insertCompletion(const QString &completion);


private:
    QString textUnderCursor() const;


private:
    QCompleter * m_completer;
};


} // namespace gloperate_qt
