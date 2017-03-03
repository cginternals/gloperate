
#pragma once

#include <memory>

#include <QString>
#include <QQuickItem>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;


namespace globjects
{
    class Texture;
    class VertexArray;
    class Program;
}


namespace gloperate
{
    class Environment;
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

    const QString & path() const;
    void setPath(const QString & path);


protected:
    virtual void geometryChanged(const QRectF & newGeometry, const QRectF & oldGeometry) override;


protected:
    gloperate::Environment * m_environment; ///< Gloperate environment to which the window belongs (may be null)

    float   m_x;                ///< Viewport (X)
    float   m_y;                ///< Viewport (Y)
    float   m_width;            ///< Viewport (Width)
    float   m_height;           ///< Viewport (Height)
    float   m_devicePixelRatio; ///< Number of device pixels per virtual pixel
    bool    m_initialized;      ///< 'true' if the canvas has been initialized, else 'false'
    QString m_path;             ///< Path to the displayed texture slot

    std::unique_ptr<globjects::VertexArray> m_vao;     ///< Screen-aligned quad geometry
    std::unique_ptr<globjects::Program>     m_program; ///< Shader program
    std::unique_ptr<globjects::Texture>     m_texture; ///< Texture that is displayed
};


} // namespace gloperate_qtquick
