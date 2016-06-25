
#pragma once


#include <QString>
#include <QQuickItem>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;

namespace gloperate {
    class AbstractCanvas;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class GLOPERATE_QTQUICK_API RenderItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString stage READ stage WRITE setStage)


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
    *    Canvas that renders into the item (can be null)
    */
    gloperate::AbstractCanvas * canvas() const;


protected:
    QString stage() const;
    void setStage(const QString & name);
    void onWindowChanged(QQuickWindow * window);
    void onBeforeRendering();


protected:
    virtual void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;
    virtual void keyPressEvent(QKeyEvent * event) override;
    virtual void keyReleaseEvent(QKeyEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;


protected:
    gloperate::AbstractCanvas * m_canvas;           ///< Canvas that renders into the item (must NOT be null)
    float                       m_devicePixelRatio; ///< Number of device pixels per virtual pixel
    bool                        m_initialized;      ///< 'true' if the canvas has been initialized, else 'false'
    QString                     m_stage;            ///< Name of the render stage to use
};


} // namespace gloperate_qtquick
