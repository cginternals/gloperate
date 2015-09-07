
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <signalzeug/Signal.h>

#include <globjects/base/CachedValue.h>

#include <gloperate/painter/AbstractCameraCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractCameraCapability
*/
class GLOPERATE_API CameraCapability : public AbstractCameraCapability
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] eye
    *    Camera position
    *  @param[in] center
    *    Look-at position
    *  @param[in] up
    *    Up-vector
    */
    CameraCapability(
        const glm::vec3 & eye = glm::vec3(0.0, 0.0, 1.0),
        const glm::vec3 & center = glm::vec3(0.0, 0.0, 0.0),
        const glm::vec3 & up = glm::vec3(0.0, 1.0, 0.0));

    /**
    *  @brief
    *    Destructor
    */
    virtual ~CameraCapability();

    /**
    *  @brief
    *    Check if camera is updated automatically
    *
    *  @return
    *    'true' if camera is updated automatically, else 'false'
    */
    bool autoUpdating() const;

    /**
    *  @brief
    *    Set if camera is updated automatically
    *
    *  @param[in] autoUpdate
    *    'true' if camera is updated automatically, else 'false'
    */
    void setAutoUpdating(bool autoUpdate);

    /**
    *  @brief
    *    Set the current camera configuration as default for resets.
    *
    *  @param[in] eye
    *    The new eye vector
    *  @param[in] center
    *    The new center vector
    *  @param[in] up
    *    The new up vector
    */
    void setDefault();
    void setDefault(const glm::vec3 & eye, const glm::vec3 & center, const glm::vec3 & up);

    /**
    *  @brief
    *    Update camera matrices
    */
    void update() const;

    // Virtual functions from AbstractCameraCapability
    virtual void reset() override;
    virtual const glm::vec3 & eye() const override;
    virtual void setEye(const glm::vec3 & eye) override;
    virtual const glm::vec3 & center() const override;
    virtual void setCenter(const glm::vec3 & center) override;
    virtual const glm::vec3 & up() const override;
    virtual void setUp(const glm::vec3 & up) override;
    virtual const glm::mat4 & view() const override;
    virtual const glm::mat4 & viewInverted() const override;
    virtual const glm::mat3 & normal() const override;


protected:
    /**
    *  @brief
    *    Mark data dirty
    *
    *  @param[in] update
    *    Update matrices immediately (true), or later (false)?
    *
    *  @remarks
    *    After calling this function, the matrices will be recalculated the next time they're accessed.
    */
    void dirty(bool update = true);

    /**
    *  @brief
    *    Invalidate matrices, causing them to be recalculated
    */
    void invalidateMatrices() const;


protected:
    // Internal data
    mutable bool m_dirty;   /**< Has the data been changed? If true, matrices will be recalculated */
    bool m_autoUpdate;      /**< 'true' if camera is updated automatically, else 'false' */

    // Camera data
    glm::vec3 m_eye;        /**< Camera position */
    glm::vec3 m_center;     /**< Look-at position */
    glm::vec3 m_up;         /**< Up-vector */

    glm::vec3 m_defaultEye;        /**< Default camera position */
    glm::vec3 m_defaultCenter;     /**< Default look-at position */
    glm::vec3 m_defaultUp;         /**< Default up-vector */

    // Camera matrices
    globjects::CachedValue<glm::mat4> m_view;           /**< View matrix */
    globjects::CachedValue<glm::mat4> m_viewInverted;   /**< Inverted view matrix */
    globjects::CachedValue<glm::mat3> m_normal;         /**< Normal matrix */
};


} // namespace gloperate
