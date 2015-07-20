
#pragma once


#include <gloperate/painter/AbstractVirtualTimeCapability.h>


namespace gloperate
{


/**
*  @brief
*    Default implementation for AbstractVirtualTimeCapability
*/
class GLOPERATE_API VirtualTimeCapability : public AbstractVirtualTimeCapability
{
public:
    /**
    *  @brief
    *    Constructor
    */
    VirtualTimeCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VirtualTimeCapability();

    // Virtual functions from AbstractVirtualTimeCapability
    virtual bool enabled() const override;
    virtual void setEnabled(bool enabled) override;
    virtual float time() const override;
    virtual float delta() const override;
    virtual void setLoopDuration(float duration) override;
    virtual void update(float delta) override;


protected:
    /**
    *  @brief
    *    Normalize time by wrapping it at the loop duration
    */
    void normalizeTime();


protected:
    bool  m_enabled;    /**< Is virtual time enabled? */
    float m_duration;   /**< Duration after which time is reset to 0 (in seconds) */ 
    float m_time;       /**< Current time (in seconds) */
    float m_delta;      /**< Last time delta  (in seconds)*/
};


} // namespace gloperate
