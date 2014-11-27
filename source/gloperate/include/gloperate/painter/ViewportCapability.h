#pragma once

#include <gloperate/gloperate_api.h>

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

    /**
    *  @brief
    *    Get viewport position x
    *
    *  @return
    *    X-position
    */
    virtual int x() const override;

    /**
    *  @brief
    *    Get viewport position y
    *
    *  @return
    *    Y-position
    */
    virtual int y() const override;

    /**
    *  @brief
    *    Get viewport width
    *
    *  @return
    *    Width
    */
    virtual int width() const override;

    /**
    *  @brief
    *    Get viewport height
    *
    *  @return
    *    Height
    */
    virtual int height() const override;

    /**
    *  @brief
    *    Set viewport
    *
    *  @param[in] x
    *    X-position
    *  @param[in] y
    *    Y-position
    *  @param[in] width
    *    Width
    *  @param[in] height
    *    Height
    */
    virtual void setViewport(int x, int y, int width, int height) override;

protected:
    int m_x;      /**< Viewport position x */
    int m_y;      /**< Viewport position y */
    int m_width;  /**< Viewport width */
    int m_height; /**< Viewport height */
};

} // namespace gloperate
