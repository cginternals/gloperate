
#pragma once


#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Pair of values that describe a range
*/
class GLOPERATE_API Range
{
public:
    /**
    *  @brief
    *    Constructor
    */
    Range();

    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] minValue
    *    Minimum value
    *  @param[in] maxValue
    *    Maximum value
    */
    Range(float minValue, float maxValue);

    /**
    *  @brief
    *    Get minimum value
    *
    *  @return
    *    Minimum value
    */
    float minimumValue() const;

    /**
    *  @brief
    *    Set minimum value
    *
    *  @param[in] minValue
    *    Minimum value
    */
    void setMinimumValue(float minValue);

    /**
    *  @brief
    *    Get maximum value
    *
    *  @return
    *    Maximum value
    */
    float maximumValue() const;

    /**
    *  @brief
    *    Set maximum value
    *
    *  @param[in] maxValue
    *    Maximum value
    */
    void setMaximumValue(float maxValue);


protected:
    float m_minValue;
    float m_maxValue;
};


} // namespace gloperate
