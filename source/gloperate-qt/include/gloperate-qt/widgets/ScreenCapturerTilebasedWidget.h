
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <QWidget>
#include <QScopedPointer>
#include <gloperate/ext-includes-end.h>

#include <gloperate-qt/gloperate-qt_api.h>


class Ui_ScreenCapturerTilebasedWidget;


namespace gloperate_qt
{


/**
*  @brief
*    UI for tilebased image saving configuration. Just a dummy yet.
*/
class GLOPERATE_QT_API ScreenCapturerTilebasedWidget : public QWidget
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    ScreenCapturerTilebasedWidget(QWidget * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScreenCapturerTilebasedWidget();


private:
    const QScopedPointer<Ui_ScreenCapturerTilebasedWidget> m_ui;    ///< UI of widget
};


} // namespace gloperate_qt
