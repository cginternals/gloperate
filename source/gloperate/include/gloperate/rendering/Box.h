
#pragma once


#include <memory>

#include <glm/vec3.hpp>

#include <globjects/Buffer.h>
#include <globjects/VertexArray.h>

#include <gloperate/rendering/AbstractDrawable.h>


namespace gloperate
{


/**
*  @brief
*    Box drawable
*/
class Box : public gloperate::AbstractDrawable
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] center
    *    Center point
    *  @param[in] size
    *    Size factor
    */
    Box(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f), float size = 1.0f);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Box();

    // Virtual AbstractDrawable functions
    virtual void draw() const override;


protected:
    std::unique_ptr<globjects::Buffer>      m_vertices;  ///< Vertex buffer
    std::unique_ptr<globjects::Buffer>      m_texCoords; ///< Texture coordinate buffer
    std::unique_ptr<globjects::VertexArray> m_vao;       ///< Vertex array object
};


} // namespace gloperate
