
#pragma once


#include <cppexpose/reflection/DynamicProperty.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Container for data objects in a pipeline
*/
template <typename T>
class Data : public cppexpose::DynamicProperty<T>
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] parent
    *    Parent object (must NOT be null!)
    *  @param[in] name
    *    Property name
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The data container is created and added to the given parent object.
    */
    Data(cppexpose::PropertyGroup * parent, const std::string & name, const T & value = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Data();

    /**
    *  @brief
    *    Check if data is required
    *
    *  @return
    *    'true' if data is required, else 'false'
    *
    *  @see
    *    setRequired
    */
    bool required() const;

    /**
    *  @brief
    *    Set if data is required
    *
    *  @param[in] required
    *    'true' if data is required, else 'false'
    *
    *  @remarks
    *    This flag plays a role for output data during the execution
    *    of pipelines. If an output data is flagged as required, the
    *    owning stage will be activated in order to produce that output.
    *    In turn, the stage will flag the input slots it need to create
    *    that output as required, too. This propagates through the entire
    *    pipeline and determines which stages will be executed.
    */
    void setRequired(bool required);


protected:
    bool m_required; ///< Is the (output) data required?
};


} // namespace cppexpose


#include <gloperate/pipeline/Data.hpp>
