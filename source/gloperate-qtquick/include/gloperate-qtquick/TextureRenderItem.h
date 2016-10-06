
#pragma once


#include <QString>
#include <QQuickItem>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Program.h>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;

namespace globjects {
    class Texture;
}

namespace gloperate {
    class Environment;
    class AbstractCanvas;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class GLOPERATE_QTQUICK_API TextureRenderItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent item (can be null)
    */
    TextureRenderItem(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureRenderItem();


protected:
    void onWindowChanged(QQuickWindow * window);
    void onAfterRendering();
    void render();
    void buildGeometry();
    void buildProgram();

    int getWindowHeight();

    QString path() const;
    void setPath(const QString & path);


protected:
    virtual void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;


protected:
    gloperate::Environment * m_environment;

    float   m_x;                ///< Viewport (X)
    float   m_y;                ///< Viewport (Y)
    float   m_width;            ///< Viewport (Width)
    float   m_height;           ///< Viewport (Height)
    float   m_devicePixelRatio; ///< Number of device pixels per virtual pixel
    bool    m_initialized;      ///< 'true' if the canvas has been initialized, else 'false'
    QString m_path;             ///< Path to the displayed texture slot

    globjects::ref_ptr<globjects::VertexArray> m_vao;     ///< Screen-aligned quad geometry
    globjects::ref_ptr<globjects::Program>     m_program; ///< Shader program
    globjects::Texture                       * m_texture; ///< Texture that is displayed
};


} // namespace gloperate_qtquick
