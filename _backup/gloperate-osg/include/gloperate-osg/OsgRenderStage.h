
#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgRenderStage_osg.cpp.


#include <glm/glm.hpp>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer 
{
    class Viewer;
    class GraphicsWindowEmbedded;
}

namespace osg 
{
    class Node;
    class Matrixd;
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
    // Input data
    gloperate::InputSlot<glm::ivec4> m_viewport;
    gloperate::InputSlot<float>      m_virtualTime;

    // Output data
    gloperate::Data<glm::mat4>       m_projectionMatrix;
    gloperate::Data<glm::mat4>       m_viewMatrix;


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
    *    Load OSG scene
    *
    *  @param[in] filename
    *    Path to OSG scene file
    */
    void loadScene(const std::string & filename);

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
    OsgKeyboardHandler * createKeyboardHandler();

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
    OsgMouseHandler * createMouseHandler();


protected:
    // Virtual AbstractRenderStage functions
    virtual void initialize() override;
    virtual void process() override;

    /**
    *  @brief
    *    Called when the viewport has been changed
    */
    virtual void handleViewportChanged();

    /**
    *  @brief
    *    Called right after OSG rendering
    */
    virtual void postOsgRendering();


protected:
    // The following functions are actually using OSG code, so they are not compatible with
    // globjects/glbinding include. Therefore, they are separately implemented in OsgPainter_osg.cpp
    void osg_loadScene(const std::string & filename);
    void osg_setScene(osg::Node * scene);
    void osg_initialize();
    void osg_process();
    void osg_cleanup();

    glm::mat4 convertMatrix(const osg::Matrixd & mat) const;


protected:
    // OSG scene data
    osgViewer::Viewer                 * m_viewer;       /**< OSG viewer */
    osgViewer::GraphicsWindowEmbedded * m_embedded;     /**< Interface that acts like a window to OSG */
    osg::Node                         * m_scene;        /**< The displayed scene */
    int                                 m_viewportX;    /**< Current viewport x */
    int                                 m_viewportY;    /**< Current viewport y */
    int                                 m_viewportW;    /**< Current viewport width */
    int                                 m_viewportH;    /**< Current viewport height */
};


} // namespace gloperate_osg
