
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
    Unknown = 0, ///< Slot of unknown type
    Input,       ///< Slot is input to a stage
    Output       ///< Slot is output of a stage
};


/**
*  @brief
*    Base class for data slots
*
*    To transfer data between stages, each stage can have a number
*    of data slots. Input slots contain the data the stage needs to
*    calculate or produce its output, the produced output data is
*    stored in its output slots. A data slot can either be set directly
*    to a specific value, or it can be connected to another slot. As long
*    as it is connected, it will return the value of the source slot.
*
*    This class should not be instanciated directly. Instead, Input<T> and
*    Output<T> should be used to declare slots for stages.
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
    *    Constructor
    *
    *  @param[in] options
    *    Options with which the property is initialized
    */
    AbstractSlot(const cppexpose::Variant & options);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractSlot();

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
    *    Check if slot is dynamic
    *
    *  @return
    *    'true' if slot has been added dynamically, else 'false'
    */
    bool isDynamic() const;

    /**
    *  @brief
    *    Check if slot is required
    *
    *  @return
    *    'true' if slot is required, else 'false'
    *
    *  @see
    *    setRequired
    */
    bool isRequired() const;

    /**
    *  @brief
    *    Set if slot is required
    *
    *  @param[in] required
    *    'true' if slot is required, else 'false'
    *
    *  @remarks
    *    This flag plays a role during the execution of pipelines.
    *    If an output slot is flagged as required, the owning stage
    *    will be activated in order to produce that output. In turn,
    *    the stage will also flag its input slots as required. This
    *    propagates through the entire pipeline and determines
    *    which stages will be executed.
    */
    void setRequired(bool required);

    /**
    *  @brief
    *    Check if slot currently has a feedback connection
    *
    *  @return
    *    'true' if slot has a feedback connection, else 'false'
    *
    *  @remarks
    *    In complex pipelines, stages sometimes have to depend on output
    *    values of stages that are processed later in the pipeline, which
    *    introduces a cyclic dependency between the stages. Therefore,
    *    a slot can be flagged as a feedback connection, indicating that
    *    the connection shall be ignored when calculating the dependency
    *    graph (and order of execution) of the pipeline.
    */
    bool isFeedback() const;

    /**
    *  @brief
    *    Set if slot currently has a feedback connection
    *
    *  @param[in] feedback
    *    'true' if slot has a feedback connection, else 'false'
    *
    *  @see
    *    isFeedback
    */
    void setFeedback(bool feedback);

    /**
    *  @brief
    *    Check if slot is connected to another slot
    *
    *  @return
    *    'true' if slot is connected, else 'false'
    */
    bool isConnected() const;

    /**
    *  @brief
    *    Check if slot is compatible to source slot
    *
    *  @param[in] source
    *    Source slot (can be null)
    *
    *  @return
    *    'true' if slot can be connected to source slot, else 'false'
    *
    *  @remarks
    *    To be compatible, the source slot must use the same data type.
    */
    virtual bool isCompatible(const AbstractSlot * source) const = 0;

    /**
    *  @brief
    *    Connect slot to source slot
    *
    *  @param[in] source
    *    Source slot (can be null)
    *
    *  @return
    *    'true' if slot could be connected, else 'false'
    */
    virtual bool connect(AbstractSlot * source) = 0;

    /**
    *  @brief
    *    Disconnect slot
    */
    virtual void disconnect() = 0;

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
    *    Invalidates data
    *
    *  @remarks
    *    The state will automatically change to valid when a new value is set.
    *
    *  @see
    *    isValid
    *  @see
    *    setValue
    */
    virtual void invalidate() = 0;

    /**
    *  @brief
    *    Check if slot was changed
    *
    *  @return
    *    'true' if data was changed, else 'false'
    *
    *  @remarks
    *    This status helps to improve performance in a pipeline.
    *    It is used to avoid processing of input values that where
    *    processed ealier already.
    */
    virtual bool hasChanged() const = 0;

    /**
    *  @brief
    *    Set the changed flag
    *
    *  @remarks
    *    This marks the input value as changed or unchanged.
    *    The state will automatically change to changed when a new value is set.
    *
    *  @see
    *    hasChanged
    */
    virtual void setChanged(bool hasChanged) = 0;

    /**
    *  @brief
    *    Called when required-flag has been changed
    */
    virtual void onRequiredChanged() = 0;

    /**
    *  @brief
    *    Called when value was invalidated i.e. it was valid and is set to invalid
    */
    virtual void onValueInvalidated() = 0;

    /**
    *  @brief
    *    Check if the Slot is a slot of any type given by the template argument list
    *
    *  @tparam Types
    *    The variadic template parameter list of all types to check
    *
    *  @return
    *    'true' if the slot is of any type given, else 'false'
    */
    template <typename... Types>
    bool isOfAnyType() const;


    // Copied from cppexpose::Object
    cppexpose::Variant toVariant() const;
    bool fromVariant(const cppexpose::Variant & value);


protected:
    /**
    *  @brief
    *    Initialize input slot
    *
    *  @param[in] slotType
    *    Type or role of the slot (input or output)
    *  @param[in] parent
    *    Parent stage (can be null)
    *  @param[in] ownership
    *    Property ownership
    */
    void initSlot(SlotType slotType, Stage * parent);


protected:
    SlotType m_slotType; ///< Type or role of the slot (input or output)
    bool     m_dynamic;  ///< 'true' if slot has been added dynamically, else 'false'
    bool     m_required; ///< Is the data required?
    bool     m_feedback; ///< Does the slot contain a feedback connection?
};


} // namespace cppexpose


#include <gloperate/pipeline/AbstractSlot.inl>
