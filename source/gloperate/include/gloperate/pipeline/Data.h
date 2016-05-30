
#pragma once


#include <cppexpose/typed/DirectValue.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Container for data objects in a pipeline
*/
template <typename T, typename BASE>
class Data : public cppexpose::DirectValue<T, BASE>
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
    Data(const T & value = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Data();
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
    virtual bool isGroup() const override;


protected:
    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Data.hpp>
