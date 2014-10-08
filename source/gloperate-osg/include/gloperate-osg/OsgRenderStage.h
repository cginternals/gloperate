#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgPainter_osg.cpp.


#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate/pipelines/InputSlot.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class Viewer;
    class GraphicsWindowEmbedded;
}
namespace osg {
    class Node;
}
namespace gloperate {
    class AbstractViewportCapability;
}


namespace gloperate_osg
{


class OsgKeyboardHandler;
class OsgMouseHandler;


/**
*  @brief
*    Rendering stage that renders an OSG scene
*/
class GLOPERATE_OSG_API OsgRenderStage : public gloperate::AbstractStage
{


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Name of the stage
    */
    OsgRenderStage(const std::string & name);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgRenderStage();

    /**
    *  @brief
    *    Get OSG viewer
    *
    *  @return
    *    OSG viewer
    */
    osgViewer::Viewer * viewer() const;

    /**
    *  @brief
    *    Get OSG scene
    *
    *  @return
    *    OSG scene
    */
    osg::Node * scene() const;

    /**
    *  @brief
    *    Set OSG scene
    *
    *  @param[in] scene
    *    OSG scene
    */
    void setScene(osg::Node * scene);

    /**
    *  @brief
    *    Create keyboard handler to control the wrapped OSG scene
    *
    *  @return
    *    Keyboard handler, can be nullptr, if the scene has not yet been initialized
    *
    *  @remarks
    *    The returned handler must be destroyed by the caller, e.g., by adding
    *    it to an InputCapability, which will take care of this automatically.
    */
    OsgKeyboardHandler * createKeyboardHandler() const;

    /**
    *  @brief
    *    Create mouse handler to control the wrapped OSG scene
    *
    *  @return
    *    Mouse handler, can be nullptr, if the scene has not yet been initialized
    *
    *  @remarks
    *    The returned handler must be destroyed by the caller, e.g., by adding
    *    it to an InputCapability, which will take care of this automatically.
    */
    OsgMouseHandler * createMouseHandler() const;


protected:
    virtual void initialize() override;
    virtual void process() override;


protected:
    // The following functions are actually using OSG code, so they are not compatible with
    // globjects/glbinding include. Therefore, they are separately implemented in OsgPainter_osg.cpp
    void osg_setScene(osg::Node * scene);
    void osg_initialize();
    void osg_process();
    void osg_cleanup();


public:
    // Input data
    gloperate::InputSlot<gloperate::AbstractViewportCapability *>  m_viewport;


protected:
    // OSG scene data
    osgViewer::Viewer                 * m_viewer;    /**< OSG viewer */
    osgViewer::GraphicsWindowEmbedded * m_embedded;  /**< Interface that acts like a window to OSG */
    osg::Node                         * m_scene;     /**< The displayed scene */
    int                                 m_viewportX; /**< Current viewport X */
    int                                 m_viewportY; /**< Current viewport Y */
    int                                 m_viewportW; /**< Current viewport width */
    int                                 m_viewportH; /**< Current viewport height */


};


} // namespace gloperate_osg
