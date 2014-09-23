/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/input/KeyboardInputHandler.h>
#include <gloperate-osg/gloperate-osg_api.h>
#include <osg/ref_ptr>


namespace osgViewer {
    class GraphicsWindowEmbedded;
}


namespace gloperate_osg
{


/**
*  @brief
*    Input handler that transfers keyboard events to OSG
*/
class GLOPERATE_OSG_API OsgKeyboardHandler : public gloperate::KeyboardInputHandler
{


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] embedded
    *    Interface that acts like a window to OSG
    */
    OsgKeyboardHandler(osgViewer::GraphicsWindowEmbedded * embedded);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgKeyboardHandler();

    virtual void onKeyDown(gloperate::Key key) override;
    virtual void onKeyUp(gloperate::Key key) override;


protected:
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_embedded; /**< Interface that acts like a window to OSG */


};


} // namespace gloperate_osg
