
#pragma once


#include <signalzeug/ScopedConnection.h>

#include <gloperate/pipeline/AbstractInputSlot.h>
#include <gloperate/pipeline/Data.h>


namespace gloperate 
{


void GLOPERATE_API printIncompatibleMessage(
    const AbstractInputSlot * slot, 
    const std::string & typeName,
    const AbstractData & data);


/**
*  @brief
*    Typed data input slot
*
*    An input slot can be connected to a data container. It is
*    informed when the connection is changed or the connected
*    data has been modified.
*
*  @see Data
*  @see AbstractStage
*  @see AbstractPipeline
*/
template <typename T>
class InputSlot : public AbstractInputSlot
{
public:
    InputSlot();

    const T & data() const;
    const T & data(const T & defaultValue) const;
    const T & operator*() const;
    const T * operator->() const;

    virtual bool connectTo(const AbstractData & data) override;
    virtual bool matchType(const AbstractData & data) override;

    template <typename U>
    const Data<U> & operator=(const Data<U> & data);

    template <typename U>
    InputSlot<T> & operator=(InputSlot<U> & slot);

    virtual const AbstractData * connectedData() const override;


protected:
    template <typename U>
    void connect(const Data<U> & data);


protected:
    const Data<T> * m_data;
    signalzeug::ScopedConnection m_connection;

    static const T s_defaultValue;
};


} // namespace gloperate


#include <gloperate/pipeline/InputSlot.hpp>
