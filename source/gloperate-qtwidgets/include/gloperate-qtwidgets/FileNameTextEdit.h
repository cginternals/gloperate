#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QPlainTextEdit>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>


class QCompleter;
class QKeyEvent;
class QFocusEvent;


namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API FileNameTextEdit : public QPlainTextEdit
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

} //namespace gloperate_qtwidgets
