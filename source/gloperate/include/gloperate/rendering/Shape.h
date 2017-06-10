
#pragma once


#include <cppassist/flags/Flags.h>

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


/**
*  @brief
*    Type of basic shapes
*/
enum class ShapeType : int
{
    None,  ///< No shape
    Point, ///< Single point
    Quad,  ///< 2D rectangle or quad
    Box,   ///< 3D box or cube
    Sphere ///< 3D sphere or ellipsoid
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


/**
*  @brief
*    Base class for basic shapes (quads, boxes, etc.)
*/
class GLOPERATE_API Shape : public AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] type
    *    Type of the shape
    *  @param[in] options
    *    Shape options
    */
    Shape(ShapeType shapeType, cppassist::Flags<ShapeOption> options);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Shape();

    /**
    *  @brief
    *    Get shape type
    *
    *  @return
    *    Type of the shape
    */
    ShapeType type() const;

    /**
    *  @brief
    *    Get shape options
    *
    *  @return
    *    Shape options
    */
    cppassist::Flags<ShapeOption> options() const;


protected:
    ShapeType                     m_type;    ///< Type of the shape
    cppassist::Flags<ShapeOption> m_options; ///< Shape options
};


} // namespace gloperate
