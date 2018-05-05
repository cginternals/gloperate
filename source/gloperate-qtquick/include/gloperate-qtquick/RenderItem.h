
#pragma once


#include <memory>

#include <QString>
#include <QTimer>
#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;

namespace globjects
{
    class Framebuffer;
}

namespace gloperate
{
    class Canvas;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class GLOPERATE_QTQUICK_API RenderItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString stage READ stage WRITE setStage)


signals:
    void canvasInitialized(); ///< Called after the canvas has been successfully initialized


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent item (can be null)
    */
    RenderItem(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItem();

    /**
    *  @brief
    *    Get canvas
    *
    *  @return
    *    Canvas that renders into the item (never null)
    */
    gloperate::Canvas * canvas() const;

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Name of render stage
    */
    const QString & stage() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] name
    *    Name of render stage
    */
    void setStage(const QString & name);

    // Virtual QQuickFramebufferObject interface
    QQuickFramebufferObject::Renderer * createRenderer() const Q_DECL_OVERRIDE;


protected:
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void hoverMoveEvent(QHoverEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;

    /**
    *  @brief
    *    Called on timer update
    */
    void onTimer();


protected:
    QString                            m_stage;  ///< Name of the render stage to use
    QTimer                             m_timer;  ///< Timer for continuous update
    std::unique_ptr<gloperate::Canvas> m_canvas; ///< Canvas that renders into the item (must NOT be null)
};


} // namespace gloperate_qtquick
