
#pragma once


#include <gloperate/pipeline/DataSlot.h>


namespace gloperate
{


/**
*  @brief
*    Output of a stage
*/
template <typename T>
class Output : public DataSlot<T>
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Property name
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] value
    *    Default value
    *
    *  @remarks
    *    The output is created and added to the given stage.
    */
    Output(const std::string & name, Stage * parent, const T & defaultValue = T());

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Output();

    // Virtual AbstractDataSlot interface
    virtual void invalidate() override;

    // Virtual AbstractSlot interface
    virtual bool isValid() const override;


protected:
    // Virtual AbstractSlot interface
    virtual void onRequiredChanged() override;

    // Virtual Typed<T> interface
    virtual void onValueChanged(const T & value) override;


protected:
    bool m_valid; ///< Does the output have a valid value?
};


} // namespace cppexpose


#include <gloperate/pipeline/Output.inl>
