
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


class GLOPERATE_QT_API FileNameTextEdit : public QPlainTextEdit
{
    Q_OBJECT


public:
    FileNameTextEdit(QWidget *parent = 0);
    ~FileNameTextEdit();

    virtual void setCompleter(QCompleter *c);
    virtual QCompleter *completer() const;


protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void focusInEvent(QFocusEvent *e);


private slots:
    virtual void insertCompletion(const QString &completion);


private:
    QString textUnderCursor() const;


private:
    QCompleter * m_completer;
};


} // namespace gloperate_qt
