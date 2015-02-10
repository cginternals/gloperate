#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_ScreenshotWidget;
class QAbstractButton;
class QPushButton;
class QDir;

namespace gloperate
{

class ResourceManager;
class Painter;
class ScreenshotTool;

}

namespace gloperate_qt
{

class QtOpenGLWindow;

}

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API ScreenshotWidget : public QWidget
{
	Q_OBJECT

public:
	ScreenshotWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
	virtual ~ScreenshotWidget();

protected:
	void handleSave(QAbstractButton* button);
	void browseDirectory(bool checked);
	void updateDirectory();

protected:
	gloperate::ScreenshotTool * m_screenshotTool;
	gloperate_qt::QtOpenGLWindow * m_context;

private:
	const QScopedPointer<Ui_ScreenshotWidget> m_ui;
	QString m_dirName;
	

};

} //namespace gloperate_qtwidgets
