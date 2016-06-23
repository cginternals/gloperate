
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
    */
    AbstractSlot();

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
    *    Get parent stage
    *
    *  @return
    *    Stage that contains the slot (can be null)
    */
    Stage * parentStage() const;

    /**
    *  @brief
    *    Get qualified name
    *
    *  @return
    *    Name with all parent names, separated by '.'
    */
    std::string qualifiedName() const;

    /**
    *  @brief
    *    Check if data is required
    *
    *  @return
    *    'true' if data is required, else 'false'
    *
    *  @see
    *    setRequired
    */
    bool isRequired() const;

    /**
    *  @brief
    *    Set if data is required
    *
    *  @param[in] required
    *    'true' if data is required, else 'false'
    *
    *  @remarks
    *    This flag plays a role during the execution of pipelines.
    *    If an output slot is flagged as required, the owning stage
    *    will be activated in order to produce that output. In turn,
    *    the stage will flag the input slots it needs to create that
    *    output as required, too. This propagates through the entire
    *    pipeline and determines which stages will be executed.
    */
    void setRequired(bool required);

    /**
    *  @brief
    *    Check if slot has valid data
    *
    *  @return
    *    'true' if data is valid, else 'false'
    *
    *  @remarks
    *    This status is important for the pipeline to work.
    *    If output data is invalid, the pipeline tries to produce
    *    it, otherwise, the pipeline is not run. This function
    *    indicates whether data needs to be updated.
    */
    virtual bool isValid() const = 0;

    /**
    *  @brief
    *    Called when required-flag has been changed
    */
    virtual void onRequiredChanged() = 0;


protected:
    SlotType m_slotType; ///< Slot type
    bool     m_required; ///< Is the data required?
};


} // namespace cppexpose
