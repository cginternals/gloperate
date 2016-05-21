
#pragma once


#include <cppexpose/typed/GetTyped.h>
#include <cppexpose/signal/Signal.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/pipeline/AbstractSlot.h>


namespace gloperate
{


class Stage;

template <typename T>
class Input;

template <typename T>
class Parameter;

template <typename T>
class Output;

template <typename T>
class ProxyOutput;


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
    *    Connect input slot to input
    *
    *  @param[in] source
    *    Input (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    bool connect(Input<T> * source);

    /**
    *  @brief
    *    Connect input slot to parameter
    *
    *  @param[in] source
    *    Parameter (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    bool connect(Parameter<T> * source);

    /**
    *  @brief
    *    Connect input slot to output
    *
    *  @param[in] source
    *    Output (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    bool connect(Output<T> * source);

    /**
    *  @brief
    *    Connect input slot to proxy output
    *
    *  @param[in] source
    *    Proxy output (can be null)
    *
    *  @return
    *    'true' if input slot could be connected, else 'false'
    */
    bool connect(ProxyOutput<T> * source);

    // Virtual AbstractInputSlot interface
    virtual const AbstractSlot * source() const override;
    virtual bool isCompatible(const AbstractSlot * source) const override;
    virtual bool connect(AbstractSlot * source) override;
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
    union {
        Input<T>                * input;          ///< Connected input (can be null)
        Parameter<T>            * parameter;      ///< Connected parameter (can be null)
        Output<T>               * output;         ///< Connected output (can be null)
        ProxyOutput<T>          * proxyOutput;    ///< Connected proxy output (can be null)
    } m_source;
    SlotType                      m_sourceType;   ///< Contains to which kind of data the input slot is connected
    cppexpose::ScopedConnection   m_connection;   ///< Connection to changed-signal of source property
};


} // namespace cppexpose


#include <gloperate/pipeline/InputSlot.hpp>
