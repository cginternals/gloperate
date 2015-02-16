#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

#include <memory>

class Ui_ImageExporterWidget;
class QAbstractButton;
class QPushButton;
class QDir;
class QDoubleSpinBox;

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
	ResolutionState(double d, QString s) :value{ d }, type{ s } {}
	double value;
	QString type;
};

class GLOPERATE_QTWIDGETS_API ImageExporterWidget : public QWidget
{
	Q_OBJECT

public:
	ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
	virtual ~ImageExporterWidget();

signals:
	void filenameChanged(const QString& text);

protected:
	void handleSave(bool checked);

	void restoreSettings();
	void initializeResolutionGroupBox();
	void updateResolutionSummary();

	void checkFilename(const QString& text);
	void saveFilename();
	std::string replaceTags(const std::string& filename);
	std::string buildFileName();
	void browseDirectory(bool checked);
	void updateDirectory();
	void updateFilenamePreview(const QString& text);

	void widthUnitChanged(const QString& text);
	void heightUnitChanged(const QString& text);
	void resolutionUnitChanged(const QString& text);
	void enableResolution(bool enable);
	void widthValueChanged(double d);
	void heightValueChanged(double d);
	void resolutionValueChanged(int i);

	double inchToPixels(double value);
	double cmToPixels(double value);
	double toPixels(double value, const QString& type);
	double pixelsToCm(double value);
	double pixelsToInch(double value);
	double pixelsTo(double value, const QString& type);

	void setDecimals(QDoubleSpinBox* box, int dec);

protected:
	gloperate::ImageExporter * m_imageExporter;
	gloperate_qt::QtOpenGLWindow * m_context;
	std::map<const QString, const QString> m_supportedTags;

private:
	const QScopedPointer<Ui_ImageExporterWidget> m_ui;
	QString m_dirName;
	int m_fileCounter;
	std::unique_ptr<ResolutionState> m_widthState;
	std::unique_ptr<ResolutionState> m_heightState;
	std::unique_ptr<ResolutionState> m_resolutionState;
	

};

} //namespace gloperate_qtwidgets
