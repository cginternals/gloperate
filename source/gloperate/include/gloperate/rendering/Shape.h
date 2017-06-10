
#pragma once


#include <map>

#include <cppexpose/reflection/Property.h>

#include <gloperate/rendering/AbstractDrawable.h>
#include <gloperate/rendering/ShapeType.h>

#include <cppassist/flags/Flags.h> // Must be included last


namespace gloperate
{


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
