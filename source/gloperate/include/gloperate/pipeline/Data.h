
#pragma once


#include <cppexpose/reflection/DynamicProperty.h>

#include <gloperate/pipeline/AbstractData.h>


namespace gloperate
{


/**
*  @brief
*    Container for data objects in a pipeline
*/
template <typename T>
class Data : public cppexpose::DynamicProperty<T, AbstractData>
{
public:
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
};


} // namespace cppexpose


#include <gloperate/pipeline/Data.hpp>
