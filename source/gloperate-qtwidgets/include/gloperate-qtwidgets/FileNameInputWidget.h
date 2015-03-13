#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QRect>
#include <memory>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_FileNameInputWidget;
class QKeyEvent;
class QString;

namespace gloperate_qtwidgets
{

class FileNameTagCompleter;
class FileNameTextEdit;

class GLOPERATE_QTWIDGETS_API FileNameInputWidget : public QWidget
{
    Q_OBJECT

public:
    FileNameInputWidget(FileNameTagCompleter * completer, QString & initialFilename, QWidget * parent = nullptr);
    virtual ~FileNameInputWidget();

protected:
    virtual void closeEvent(QCloseEvent * event);

protected:
    std::shared_ptr<FileNameTagCompleter> m_completer;

private:
    const QScopedPointer<Ui_FileNameInputWidget> m_ui;
};

} //namespace gloperate_qtwidgets
