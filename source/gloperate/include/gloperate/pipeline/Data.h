
#pragma once


#include <string>

#include <gloperate/pipeline/AbstractData.h>


namespace gloperate 
{


/**
*  @brief
*    Typed data container
*
*    A data container holds a typed data object. It can be
*    connected to an input slot and helps to identify which
*    and when data has been modified. It is used in the context
*    of pipelines to promote data and control the order and
*    execution of pipeline stages based on data flow.
*
*  @see InputSlot
*  @see AbstractStage
*  @see AbstractPipeline
*/
template <typename T>
class Data : public AbstractData
{
public:
    template <typename... Args>
    explicit Data(Args&&... args);

    T & data();
    const T & data() const;

    T & operator*();
    const T & operator*() const;
    T * operator->();
    const T * operator->() const;

    operator const T &() const;

    Data<T> & operator=(const Data<T> & data);
    const T & operator=(const T & value);

    void setData(const T & value);

    virtual std::string type() const override;
    
protected:
    T m_data;
};


} // namespace gloperate


#include <gloperate/pipeline/Data.hpp>
