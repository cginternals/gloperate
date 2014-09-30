#pragma once


// Do not include any osg headers OR gloperate headers that pull in globjects/glbinding,
// because that would break the separation between glbinding and gl.h.
// Use only pointers to OSG objects, call ref/unref to maintain memory management,
// and separate all calls to osg in osg_ functions implemented inside OsgPainter_osg.cpp.


#include <gloperate/pipelines/AbstractStage.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class Viewer;
    class GraphicsWindowEmbedded;
}
namespace osg {
    class Node;
}


namespace gloperate_osg
{


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


protected:
    osgViewer::Viewer                 * m_viewer;   /**< OSG viewer */
    osgViewer::GraphicsWindowEmbedded * m_embedded; /**< Interface that acts like a window to OSG */
    osg::Node                         * m_scene;    /**< The displayed scene */


};


} // namespace gloperate_osg
