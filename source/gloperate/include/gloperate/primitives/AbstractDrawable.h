
#pragma once


#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class VertexArray;
}


namespace gloperate
{


/**
*  @brief
*    Base class for drawables
*/
class GLOPERATE_API AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractDrawable();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractDrawable();

    /**
    *  @brief
    *    Get vertex array
    *
    *  @return
    *    Vertex array
    */
    globjects::VertexArray * vertexArray() const;

    /**
    *  @brief
    *    Draw geometry
    */
    virtual void draw() = 0;


protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;       /**< Vertex array object */
};


} // namespace gloperate
