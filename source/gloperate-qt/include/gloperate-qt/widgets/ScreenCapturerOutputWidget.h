
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


/**
*  @brief
*    UI for output configuration of image saving.
*/
class GLOPERATE_QT_API ScreenCapturerOutputWidget : public QWidget
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Resource maanger used by viewer
    *  @param[in] painter
    *    Painter active in viewer
    *  @param[in] context
    *    OpenGl context of viewer
    *  @param[in] screenCapturer
    *    ScreenCapturer tool managed by UI
    *  @param[in] parent
    *    Parent widget
    */
    ScreenCapturerOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, gloperate::ScreenCapturer * screenCapturer, QWidget * parent = nullptr);
    
    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScreenCapturerOutputWidget();

public slots:
    /**
    *  @brief
    *    Update resolution summary label
    *
    *  @param[in] resolution
    *    New resolution string
    */
    void updateResolutionSummaryLabel(const std::string& resolution);


signals:
    /**
    *  @brief
    *    Emitted when filename changed
    */
    void filenameChanged();


private:
    void handleSave(bool);

    void restoreSettings();
    void initializeFileNameTextEdit();

    void saveFilename();
    std::string buildFileName();
    void browseDirectory(bool);
    void updateDirectory();
    void updateFilenamePreview();
    void checkFilename();
    void updateUiFileNameEnumIndex();


private:
    gloperate::ScreenCapturer * m_screenCapturer;
    gloperate_qt::QtOpenGLWindow * m_context;
    const QScopedPointer<Ui_ScreenCapturerOutputWidget> m_ui;
};


} // namespace gloperate_qt
