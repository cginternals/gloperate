
#pragma once


#include <cppexpose/reflection/DynamicProperty.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Container for data objects in a pipeline
*/
template <typename T, typename BASE>
class Data : public cppexpose::DynamicProperty<T, BASE>
{
public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] name
    *    Property name
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The data container is created and added to the given stage.
    */
    Data(Stage * parent, const std::string & name, const T & defaultValue = T());

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

    // Virtual AbstractSlot interface
    virtual bool isValid() const override;
};


} // namespace cppexpose


#include <gloperate/pipeline/Data.hpp>
