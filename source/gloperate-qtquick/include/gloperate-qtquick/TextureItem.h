
#pragma once


#include <memory>

#include <QString>
#include <QQuickFramebufferObject>

#include <gloperate-qtquick/gloperate-qtquick_api.h>


class QQuickWindow;

namespace globjects
{
    class Framebuffer;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class GLOPERATE_QTQUICK_API TextureItem : public QQuickFramebufferObject
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
    TextureItem(QQuickItem * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureItem();

    /**
    *  @brief
    *    Get path
    *
    *  @return
    *    Path to texture slot
    */
    const QString & path() const;

    /**
    *  @brief
    *    Set path
    *
    *  @param[in] path
    *    Path to texture slot
    */
    void setPath(const QString & path);

    // Virtual QQuickFramebufferObject interface
    QQuickFramebufferObject::Renderer * createRenderer() const Q_DECL_OVERRIDE;


protected:
    QString m_path; ///< Path to texture slot
};


} // namespace gloperate_qtquick
