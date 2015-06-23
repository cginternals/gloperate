#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_ImageExporterOutputWidget;
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

class GLOPERATE_QTWIDGETS_API ImageExporterOutputWidget : public QWidget
{
    Q_OBJECT

public:
    ImageExporterOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
    virtual ~ImageExporterOutputWidget();

    void updateResolutionSummaryLabel(const QString& sizeSummary);
    void updateResolution(const QSize& resolution);

signals:
    void filenameChanged();

protected:
    void handleSave(bool);

    void restoreSettings();
    void initializeFileNameTextEdit();

    void checkFilename();
    void saveFilename();
    std::string replaceTags(const std::string& filename, bool shouldUpdateUiFilename = true);
    std::string buildFileName();
    void browseDirectory(bool);
    void updateDirectory();
    void updateFilenamePreview();
    void updateUiFileName();
    std::string extractEnumNumStartIndex(const std::string& filename, int position);

protected:
    gloperate::ImageExporter * m_imageExporter;
    gloperate_qt::QtOpenGLWindow * m_context;
    std::map<const QString, const QString> m_supportedTags;

private:
    const QScopedPointer<Ui_ImageExporterOutputWidget> m_ui;
    QString m_dirName;
    QScopedPointer<QSize> m_resolution;

};

} //namespace gloperate_qtwidgets
