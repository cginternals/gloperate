
#pragma once


#include <map>

#include <cppexpose/reflection/Property.h>


namespace gloperate
{


/**
*  @brief
*    Type of basic shapes
*/
enum class ShapeType : int
{
    None,     ///< No shape
    Point,    ///< Single point
    Triangle, ///< 2D triangle
    Quad,     ///< 2D rectangle or quad
    Box,      ///< 3D box or cube
    Sphere    ///< 3D sphere or ellipsoid
};

/**
*  @brief
*    Options for basic shapes
*/
enum class ShapeOption : int
{
    None = 0,            ///< No option
    IncludeTexCoords = 1 ///< Include texture coordinates in geometry
};


} // namespace gloperate


namespace cppexpose
{


template <>
class EnumValues<gloperate::ShapeType>
{
public:
    static std::map<std::string, gloperate::ShapeType> namedValues()
    {
        std::map<std::string, gloperate::ShapeType> values;
        values["None"]     = gloperate::ShapeType::None;
        values["Point"]    = gloperate::ShapeType::Point;
        values["Quad"]     = gloperate::ShapeType::Quad;
        values["Triangle"] = gloperate::ShapeType::Triangle;
        values["Box"]      = gloperate::ShapeType::Box;
        values["Sphere"]   = gloperate::ShapeType::Sphere;

        return values;
    }
};


} // namespace cppexpose
