
#pragma once


#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for data slots
*
*    Data slots are slots that contain actual data.
*    In the current pipeline architecture, there are two
*    types of data slots: outputs and parameters.
*/
class GLOPERATE_API AbstractDataSlot : public AbstractSlot
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractDataSlot();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDataSlot();

    /**
    *  @brief
    *    Set validity of data
    *
    *  @remarks
    *    This marks the output value as valid or invalid.
    *    The state will automatically change to valid when a new value is set.
    *
    *  @see
    *    isValid
    *  @see
    *    setValue
    */
    virtual void setValid(bool isValid) = 0;


protected:
    /**
    *  @brief
    *    Initialize data slot
    *
    *  @param[in] type
    *    Slot type
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initDataSlot(SlotType type, Stage * parent, cppexpose::PropertyOwnership ownership);
};


} // namespace cppexpose
