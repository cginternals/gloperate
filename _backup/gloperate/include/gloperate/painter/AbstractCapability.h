
#pragma once


#include <signalzeug/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate 
{


/**
*  @brief
*    Base class for painter capabilities
*
*    Capabilities are used by painters to communicate their behaviour and supported interfaces.
*    For example, to receive continous updates and timing information, a painter would register
*    a VirtualTimeCapability. To signal that a painter is able to render into a supplied frame buffer
*    object, it would register a TargetFramebufferCapability. All Capabilities have a flag if their
*    value has changed that can be regarded by the using painter. Capabilities are used mainly as a
*    communication object between a painter and the outside world, it is not intended to extend
*    the functionality of a painter.
*/
class GLOPERATE_API AbstractCapability
{
public:
    signalzeug::Signal<> changed;   /**< Called when the capability information has been changed */


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractCapability();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCapability();

    /**
    *  @brief
    *    Check if the information of this capability have changed
    *
    *  @return
    *    'true' if capability has been changed, else 'false'
    *
    *  @remarks
    *    This functionality can be used by a painter to determine, whether capability
    *    data has been changed since the last call. It is the responsibility of the
    *    painter to react on that change and reset this flag by calling setChanged(false).
    *    For example, the viewport capability will be marked as 'changed' whenever a
    *    new viewport is set.
    */
    bool hasChanged() const;

    /**
    *  @brief
    *    Set if the information of this capability have changed
    *
    *  @param[in] changed
    *    'true' if capability has been changed, else 'false'
    */
    void setChanged(bool changed);

    template <typename T>
    bool is() const;

    template <typename T>
    T* as();


protected:
    bool m_changed;     /**< Has the capability information been changed? */
};


} // namespace gloperate


#include <gloperate/painter/AbstractCapability.hpp>
