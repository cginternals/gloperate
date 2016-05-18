
#pragma once


#include <cppexpose/typed/GetTyped.h>
#include <cppexpose/signal/Signal.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/pipeline/Output.h>


namespace gloperate
{


class Stage;


/**
*  @brief
*    Input slot that can be connected to a data source
*
*  @see
*    Data
*/
template <typename T, typename BASE>
class InputSlot : public cppexpose::GetTyped<T, BASE>::Type
{
public:
    cppexpose::Signal<const T &> valueChanged;  ///< Called when the value has been changed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] name
    *    Property name
    *  @param[in] defaultValue
    *    Default value
    *
    *  @remarks
    *    The input slot is created and added to the given stage.
    */
    InputSlot(Stage * parent, const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~InputSlot();

    /**
    *  @brief
    *    Connect input slot to output data
    *
    *  @param[in] source
    *    Output (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    bool connect(const Output<T> * source);

    // Virtual AbstractInputSlot interface
    virtual bool isCompatible(const cppexpose::AbstractProperty * source) const override;
    virtual const cppexpose::AbstractProperty * source() const override;
    virtual bool connect(const cppexpose::AbstractProperty * source) override;
    virtual void disconnect() override;

    // Virtual AbstractTyped interface
    virtual cppexpose::AbstractTyped * clone() const override;

    // Virtual Typed<T> interface
    virtual T value() const override;
    virtual void setValue(const T & value) override;
    virtual const T * ptr() const override;
    virtual T * ptr() override;

    // Virtual AbstractProperty interface
    virtual bool isGroup() const override;


protected:
    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;


protected:
    T                             m_defaultValue; ///< Default value that is returned if unconnected
    const Output<T>             * m_source;       ///< Connected source (can be null)
    cppexpose::ScopedConnection   m_connection;   ///< Connection to changed-signal of source property
};


} // namespace cppexpose


#include <gloperate/pipeline/InputSlot.hpp>
