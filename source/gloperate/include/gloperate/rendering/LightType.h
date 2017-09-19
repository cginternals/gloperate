
#pragma once


namespace gloperate
{

/**
*  @brief
*    LightType enum for light specification
*/
enum LightType {
    None = 0,
    Ambient,
    Directional,
    Point,
    PointAttenuated
};


} // namespace gloperate


namespace cppexpose
{


/**
*  @brief
*    Template specialization of enum strings for LightType.
*/
template<>
struct EnumDefaultStrings<gloperate::LightType>
{
    std::map<gloperate::LightType, std::string> operator()()
    {
        return{
            { gloperate::LightType::None, "None" },
            { gloperate::LightType::Ambient, "Ambient" },
            { gloperate::LightType::Directional, "Directional" },
            { gloperate::LightType::Point, "Point" },
            { gloperate::LightType::PointAttenuated, "PointAttenuated" }
        };
    }
};


} // namespace cppexpose
