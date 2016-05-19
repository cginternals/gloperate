
#pragma once


#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Slot type
*/
enum class SlotType
{
    Empty = 0,
    Input,
    Parameter,
    Output,
    ProxyOutput
};


/**
*  @brief
*    Base class for slots on stages (e.g., inputs, outputs, ...)
*/
class GLOPERATE_API AbstractSlot : public cppexpose::AbstractProperty
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Slot type
    */
    AbstractSlot(SlotType type);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractSlot();

    /**
    *  @brief
    *    Get slot type
    *
    *  @return
    *    Slot type
    */
    SlotType slotType() const;

    /**
    *  @brief
    *    Get owner stage
    *
    *  @return
    *    Stage that owns the data container (can be null)
    */
    Stage * owner() const;

    /**
    *  @brief
    *    Get qualified name
    *
    *  @return
    *    Name with all parent names, separated by '.'
    */
    std::string qualifiedName() const;


protected:
    SlotType   m_slotType; ///< Slot type
    Stage    * m_owner;    ///< Stage that owns the slot (can be null)
};


} // namespace cppexpose
