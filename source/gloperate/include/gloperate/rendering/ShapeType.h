
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
struct EnumDefaultStrings<gloperate::ShapeType>
{
    std::map<gloperate::ShapeType, std::string> operator()()
    {
        std::map<gloperate::ShapeType, std::string> values;
        values[gloperate::ShapeType::None]     = "None";
        values[gloperate::ShapeType::Point]    = "Point";
        values[gloperate::ShapeType::Quad]     = "Quad";
        values[gloperate::ShapeType::Triangle] = "Triangle";
        values[gloperate::ShapeType::Box]      = "Box";
        values[gloperate::ShapeType::Sphere]   = "Sphere";

        return values;
    }
};


} // namespace cppexpose
