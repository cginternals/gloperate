
#pragma once


#include <cppexpose/typed/DirectValue.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/pipeline/AbstractDataSlot.h>


namespace gloperate
{


/**
*  @brief
*    Container for data slots in a pipeline
*/
template <typename T>
class DataSlot : public cppexpose::DirectValue<T, AbstractDataSlot>
{
public:
    cppexpose::Signal<const T &> valueChanged;  ///< Called when the value has been changed


public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The data container is created and added to the given stage.
    */
    DataSlot(const T & value = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DataSlot();
    //@}

    //@{
    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Reference to the stored data object
    */
    T & operator*();
    const T & operator*() const;
    //@}

    //@{
    /**
    *  @brief
    *    Dereference pointer operator
    *
    *  @return
    *    Pointer to the stored data object (never null)
    */
    T * operator->();
    const T * operator->() const;
    //@}

    // Virtual AbstractProperty interface
    virtual bool isObject() const override;
};


} // namespace cppexpose


#include <gloperate/pipeline/DataSlot.inl>
