
#pragma once


#include <cppexpose/signal/Signal.h>
#include <cppexpose/reflection/AbstractProperty.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Base class for input slots
*/
class GLOPERATE_API AbstractInputSlot : public cppexpose::AbstractProperty
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
    *    Check if input slot is compatible to source data
    *
    *  @param[in] source
    *    Data source (can be null)
    *
    *  @return
    *    'true' if data can be connected to input slot, else 'false'
    *
    *  @remarks
    *    To be compatible, the input property must be an instance of Data
    *    and use the same type as the input slot.
    */
    virtual bool isCompatible(const cppexpose::AbstractProperty * source) const = 0;

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
    *    Get currently connection data source
    *
    *  @return
    *    Data source (can be null)
    */
    virtual const cppexpose::AbstractProperty * source() const = 0;

    /**
    *  @brief
    *    Connect input slot to data source
    *
    *  @param[in] source
    *    Data source (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    virtual bool connect(const cppexpose::AbstractProperty * source) = 0;

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
    *    shall be ignored when calucating the dependency graph
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
    *  @param[in] parent
    *    Parent stage (can be null)
    */
    void initInputSlot(Stage * parent);


protected:
    bool m_feedback; ///< Does the input slot contain a feedback connection?
};


} // namespace cppexpose
