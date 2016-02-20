
#pragma once


#include <QQuickItem>


class QQuickWindow;

namespace gloperate {
    class Stage;
}


namespace gloperate_qtquick
{


/**
*  @brief
*    Qt quick item for displaying a gloperate render stage
*/
class RenderItem : public QQuickItem
{
    Q_OBJECT


public:
    /**
    *  @brief
    *    Constructor
    */
    RenderItem();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderItem();


protected:
    void onWindowChanged(QQuickWindow * window);
    void onBeforeRendering();
    void onBeforeSynchronizing();
    void onSceneGraphInvalidated();


protected:
    gloperate::Stage * m_renderStage; ///< Render stage that renders into the item (can be null)
};


} // namespace gloperate_qtquick
