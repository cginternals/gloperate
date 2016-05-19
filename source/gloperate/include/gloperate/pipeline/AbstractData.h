
#pragma once


#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for data containers
*/
class GLOPERATE_API AbstractData : public AbstractSlot
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Slot type
    */
    AbstractData(SlotType type);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractData();

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
    bool required() const;

    /**
    *  @brief
    *    Set if data is required
    *
    *  @param[in] required
    *    'true' if data is required, else 'false'
    *
    *  @remarks
    *    This flag plays a role for output data during the execution
    *    of pipelines. If an output data is flagged as required, the
    *    owning stage will be activated in order to produce that output.
    *    In turn, the stage will flag the input slots it need to create
    *    that output as required, too. This propagates through the entire
    *    pipeline and determines which stages will be executed.
    */
    void setRequired(bool required);


protected:
    bool m_required; ///< Is the (output) data required?
};


} // namespace cppexpose
