/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/input/MouseInputHandler.h>
#include <gloperate-osg/gloperate-osg_api.h>


namespace osgViewer {
    class GraphicsWindowEmbedded;
}


namespace gloperate_osg
{


/**
*  @brief
*    Input handler that transfers mouse events to OSG
*/
class GLOPERATE_OSG_API OsgMouseHandler : public gloperate::MouseInputHandler
{


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] embedded
    *    Interface that acts like a window to OSG
    */
    OsgMouseHandler(osgViewer::GraphicsWindowEmbedded * embedded);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OsgMouseHandler();

    virtual void onMouseMove(int x, int y) override;
    virtual void onMousePress(int x, int y, int button) override;
    virtual void onMouseRelease(int x, int y, int button) override;
    virtual void onMouseDoubleClick(int x, int y, int button) override;


protected:
    osgViewer::GraphicsWindowEmbedded * m_embedded; /**< Interface that acts like a window to OSG */


};


} // namespace gloperate_osg
