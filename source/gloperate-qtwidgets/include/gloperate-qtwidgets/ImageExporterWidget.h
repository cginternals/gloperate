#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_ImageExporterWidget;
class QAbstractButton;
class QPushButton;
class QDir;

namespace gloperate
{

class ResourceManager;
class Painter;
class ImageExporter;

}

namespace gloperate_qt
{

class QtOpenGLWindow;

}

namespace gloperate_qtwidgets
{

struct ResolutionState{
	ResolutionState(double d, QString s, bool b) :value{ d }, type{ s }, constraintEnforced{ b } {}
	double value;
	QString type;
	bool constraintEnforced;
};

class GLOPERATE_QTWIDGETS_API ImageExporterWidget : public QWidget
{
	Q_OBJECT

public:
	ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
	virtual ~ImageExporterWidget();

protected:
	void handleSave(bool checked);
	void browseDirectory(bool checked);
	void updateDirectory();
	void restoreSettings();
	void checkFilename(const QString& text);
	void saveFilename();
	std::string buildFileName();
	void initializeResolutionGroupBox();
	void widthUnitChanged(const QString& text);
	void heightUnitChanged(const QString& text);
	void enableResolution(bool enable);
	void widthValueChanged(double d);
	void heightValueChanged(double d);

protected:
	gloperate::ImageExporter * m_imageExporter;
	gloperate_qt::QtOpenGLWindow * m_context;

private:
	const QScopedPointer<Ui_ImageExporterWidget> m_ui;
	QString m_dirName;
	int m_fileCounter;
	std::unique_ptr<ResolutionState> m_widthState;
	std::unique_ptr<ResolutionState> m_heightState;
	std::unique_ptr<ResolutionState> m_resolutionState;
	

};

} //namespace gloperate_qtwidgets
