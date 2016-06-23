
#pragma once


#include <cppexpose/signal/Signal.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for input slots
*
*    Input slots are slots that do not contain data themselves,
*    but can be connected to data slots which contain the actual
*    data. In the current pipeline architecture, there are two
*    types of input slots: inputs and proxy outputs.
*/
class GLOPERATE_API AbstractInputSlot : public AbstractSlot
{
public:
    cppexpose::Signal<> connectionChanged;  ///< Called when the connection has been changed


public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractInputSlot();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractInputSlot();

    /**
    *  @brief
    *    Check if input slot is connected to a data property
    *
    *  @return
    *    'true' if input slot is connected, else 'false'
    */
    bool isConnected() const;

    /**
    *  @brief
    *    Get currently connected slot
    *
    *  @return
    *    Source slot (can be null)
    */
    virtual const AbstractSlot * source() const = 0;

    /**
    *  @brief
    *    Check if input slot is compatible to source slot
    *
    *  @param[in] source
    *    Source slot (can be null)
    *
    *  @return
    *    'true' if slot can be connected to input slot, else 'false'
    *
    *  @remarks
    *    To be compatible, the source must use the same type as the input slot.
    */
    virtual bool isCompatible(const AbstractSlot * source) const = 0;

    /**
    *  @brief
    *    Connect input slot to source slot
    *
    *  @param[in] source
    *    Source slot (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    virtual bool connect(AbstractSlot * source) = 0;

    /**
    *  @brief
    *    Disconnect input slot
    */
    virtual void disconnect() = 0;

    /**
    *  @brief
    *    Check if input slot currently has a feedback connection
    *
    *  @return
    *    'true' if input slot has a feedback connection, else 'false'
    *
    *  @remarks
    *    In complex pipelines, stages sometimes have to depend on
    *    output values of stages that are processed later in the
    *    pipeline, which introduces a cyclic dependency between
    *    the stages. Therefore, an input slot can be flagged as
    *    a feedback connection, indicating that the connection
    *    shall be ignored when calculating the dependency graph
    *    (and order of execution) of the pipeline.
    */
    bool isFeedback() const;

    /**
    *  @brief
    *    Set if input slot currently has a feedback connection
    *
    *  @param[in] feedback
    *    'true' if input slot has a feedback connection, else 'false'
    *
    *  @see
    *    isFeedback
    */
    void setFeedback(bool feedback);


protected:
    /**
    *  @brief
    *    Initialize input slot
    *
    *  @param[in] type
    *    Slot type
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initInputSlot(SlotType type, Stage * parent, cppexpose::PropertyOwnership ownership);


protected:
    bool m_feedback; ///< Does the input slot contain a feedback connection?
};


} // namespace cppexpose
