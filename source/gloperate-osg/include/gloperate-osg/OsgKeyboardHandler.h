/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/input/KeyboardInputHandler.h>
#include <gloperate-osg/gloperate-osg_api.h>


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

    virtual void onKeyDown(unsigned int key) override;
    virtual void onKeyUp(unsigned int key) override;


protected:
    osgViewer::GraphicsWindowEmbedded * m_embedded; /**< Interface that acts like a window to OSG */


};


} // namespace gloperate_osg
