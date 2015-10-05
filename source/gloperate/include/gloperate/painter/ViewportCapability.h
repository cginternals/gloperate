
#pragma once


#include <gloperate/painter/AbstractViewportCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractViewportCapability
*/
class GLOPERATE_API ViewportCapability : public AbstractViewportCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ViewportCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ViewportCapability();

    // Virtual functions from AbstractViewportCapability
    virtual int x() const override;
    virtual int y() const override;
    virtual int width() const override;
    virtual int height() const override;
    virtual void setViewport(int x, int y, int width, int height) override;


protected:
    int m_x;      /**< Viewport position x */
    int m_y;      /**< Viewport position y */
    int m_width;  /**< Viewport width */
    int m_height; /**< Viewport height */
};


} // namespace gloperate
