/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <osgViewer/Viewer>
#include <osg/ref_ptr>
#include <osg/Node>
#include <gloperate/Painter.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace gloperate {
    class AbstractViewportCapability;
}


namespace gloperate_osg
{


/**
*  @brief
*    Painter that renders an OSG scene
*/
class GLOPERATE_OSG_API OsgPainter : public gloperate::Painter
{


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] resourceManager
    *    Resource manager, e.g., to load and save textures
    */
    OsgPainter(gloperate::ResourceManager & resourceManager);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgPainter();

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
    virtual void onInitialize() override;
    virtual void onPaint() override;


protected:
    osg::ref_ptr<osgViewer::Viewer>                 m_viewer;   /**< OSG viewer */
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_embedded; /**< Interface that acts like a window to OSG */
    osg::ref_ptr<osg::Node>                         m_scene;    /**< The displayed scene */

    // Capabilities
    gloperate::AbstractViewportCapability * m_viewportCapability;


};


} // namespace gloperate_osg
