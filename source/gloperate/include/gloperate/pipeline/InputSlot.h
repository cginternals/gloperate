
#pragma once


#include <cppexpose/reflection/TypedProperty.h>
#include <cppexpose/typed/TypeSelector.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/pipeline/Data.h>


namespace gloperate
{


/**
*  @brief
*    Input slot that can be connected to a data source
*
*  @see
*    Data
*/
template <typename T>
class InputSlot : public cppexpose::TypeSelector<T>::Type, public cppexpose::TypedProperty<T>
{
public:
    cppexpose::Signal<> connectionChanged;  ///< Called when the connection has been changed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent object (must NOT be null!)
    *  @param[in] name
    *    Property name
    *  @param[in] defaultValue
    *    Default value
    *
    *  @remarks
    *    The input slot is created and added to the given parent object.
    */
    InputSlot(cppexpose::PropertyGroup * parent, const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~InputSlot();

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
    *    To be compatible, the input property must be of type Data<T>.
    */
    bool isCompatible(const cppexpose::AbstractProperty * source) const;

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
    const Data<T> * source() const;

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
    bool connect(const cppexpose::AbstractProperty * source);

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
    bool connect(const Data<T> * source);

    /**
    *  @brief
    *    Disconnect input slot
    */
    void disconnect();

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

    // Virtual AbstractTyped interface
    virtual cppexpose::AbstractTyped * clone() const override;

    // Virtual Typed<T> interface
    virtual T value() const override;
    virtual void setValue(const T & value) override;
    virtual const T * ptr() const override;
    virtual T * ptr() override;

    // Virtual AbstractProperty interface
    virtual cppexpose::AbstractTyped * asTyped() override;
    virtual const cppexpose::AbstractTyped * asTyped() const override;
    virtual bool isGroup() const override;


protected:
    bool                          m_feedback;     ///< Does the input slot contain a feedback connection?
    T                             m_defaultValue; ///< Default value that is returned if unconnected
    const Data<T>               * m_source;       ///< Connected source (can be null)
    cppexpose::ScopedConnection   m_connection;   ///< Connection to changed-signal of source property
};


} // namespace cppexpose


#include <gloperate/pipeline/InputSlot.hpp>
