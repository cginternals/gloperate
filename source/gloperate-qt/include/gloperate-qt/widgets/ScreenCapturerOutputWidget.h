
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <QString>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ScreenCapturerOutputWidget;
class QAbstractButton;
class QPushButton;
class QDir;


namespace gloperate
{

class ResourceManager;
class Painter;
class ScreenCapturer;

}


namespace gloperate_qt
{


class QtOpenGLWindow;


class GLOPERATE_QT_API ScreenCapturerOutputWidget : public QWidget
{
    Q_OBJECT


public:
    ScreenCapturerOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget * parent = nullptr);
    virtual ~ScreenCapturerOutputWidget();

    void updateResolutionSummaryLabel(const QString& sizeSummary);
    void updateResolution(const QSize& resolution);


signals:
    void filenameChanged();


protected:
    void handleSave(bool);

    void restoreSettings();
    void initializeFileNameTextEdit();

    void saveFilename();
    std::string replaceTags(const std::string& filename, bool shouldUpdateUiFilename = true);
    std::string buildFileName();
    void browseDirectory(bool);
    void updateDirectory();
    void updateFilenamePreview();
    void updateUiFileName();
    std::string extractEnumNumStartIndex(const std::string& filename, int position);
    void checkFilename();


protected:
    gloperate::ScreenCapturer * m_screenCapturer;
    gloperate_qt::QtOpenGLWindow * m_context;


private:
    const QScopedPointer<Ui_ScreenCapturerOutputWidget> m_ui;
    QString m_dirName;
    QScopedPointer<QSize> m_resolution;
};


} // namespace gloperate_qt
