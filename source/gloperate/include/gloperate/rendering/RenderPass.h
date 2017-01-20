
#pragma once


#include <unordered_map>
#include <cstdint>
#include <cstddef>

#include <glbinding/gl/types.h>

#include <globjects/base/Referenced.h>
#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Buffer;
    class Program;
    class ProgramPipeline;
    class Sampler;
    class Texture;
    class TransformFeedback;
    class State;
}


namespace gloperate
{


class Drawable;


/**
*  @brief
*    Render pass that renders a geometry with a given set of states and modes
*
*    A render pass encapsulates the rendering of a geometry together
*    with a configuration of OpenGL states and modes which are set before
*    rendering, such as the binding of shader programs, textures, and buffers.
*    Once configured, a call to draw() will activate the specified configuration
*    and then draw the associated geometry of the render pass.
*/
class GLOPERATE_API RenderPass : public globjects::Referenced
{
public:
    /**
    *  @brief
    *    Constructor
    */
    RenderPass();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderPass();

    /**
    *  @brief
    *    Execute render pass
    */
    void draw() const;

    /**
    *  @brief
    *    Get state that is applied before rendering
    *
    *  @return
    *    State (can be null)
    */
    globjects::State * stateBefore() const;
    
    /**
    *  @brief
    *    Set state that is applied before rendering
    *
    *  @param[in] state
    *    State (can be null)
    *    
    *  @see setStateAfter()
    */
    void setStateBefore(globjects::State * state);

    /**
    *  @brief
    *    Get state that is applied after rendering
    *
    *  @return
    *    State (can be null)
    */
    globjects::State * stateAfter() const;

    /**
    *  @brief
    *    Set state that is applied after rendering
    *
    *  @param[in] state
    *    State (can be null)
    *    
    *  @remarks
    *    Use this to revert any state settings applied via setStateBefore()
    *  
    *  @see setStateBefore()
    */
    void setStateAfter(globjects::State * state);

    /**
    *  @brief
    *    Get geometry that is drawn by the render pass
    *
    *  @return
    *    Geometry (can be null)
    */
    Drawable * geometry() const;

    /**
    *  @brief
    *    Set geometry that is drawn by the render pass
    *
    *  @param[in] geometry
    *    Geometry (can be null)
    */
    void setGeometry(Drawable * geometry);

    /**
    *  @brief
    *    Get recording transform feedback object
    *
    *  @return
    *    Transform feedback object (can be null)
    */
    globjects::TransformFeedback * recordTransformFeedback() const;

    /**
    *  @brief
    *    Set recording transform feedback object
    *
    *  @param[in] transformFeedback
    *    Transform feedback object (can be null)
    *
    *  @remarks
    *    If set, the render pass geometry output will be recorded into the specified
    *    transform feedback object, rasterization to screen is disabled.
    */
    void setRecordTransformFeedback(globjects::TransformFeedback * transformFeedback);

    /**
    *  @brief
    *    Get recording transform feedback primitive mode
    *
    *  @return
    *    Primitive mode (e.g., GL_POINTS, GL_TRIANGLES, ...)
    */
    gl::GLenum recordTransformFeedbackMode() const;

    /**
    *  @brief
    *    Set recording transform feedback primitive mode
    *
    *  @param[in] mode
    *    Primitive mode (e.g., GL_POINTS, GL_TRIANGLES, ...)
    */
    void setRecordTransformFeedbackMode(gl::GLenum mode);

    /**
    *  @brief
    *    Get playback transform feedback object
    *
    *  @return
    *    Transform feedback object (can be null)
    */
    globjects::TransformFeedback * drawTransformFeedback() const;

    /**
    *  @brief
    *    Set playback transform feedback object
    *
    *  @param[in] transformFeedback
    *    Transform feedback object (can be null)
    *
    *  @remarks
    *    If set, the render pass will draw the geometry from the transform
    *    feedback object instead of rendering the associated drawable.
    *    A recording transform feedback object can be active at the same time, but
    *    must not share the same buffer with the transform feedback playback object.
    */
    void setDrawTransformFeedback(globjects::TransformFeedback * transformFeedback);

    /**
    *  @brief
    *    Get playback transform feedback primitive mode
    *
    *  @return
    *    Primitive mode (e.g., GL_POINTS, GL_TRIANGLES, ...)
    */
    gl::GLenum drawTransformFeedbackMode() const;

    /**
    *  @brief
    *    Set playback transform feedback primitive mode
    *
    *  @param[in] mode
    *    Primitive mode (e.g., GL_POINTS, GL_TRIANGLES, ...)
    */
    void setDrawTransformFeedbackMode(gl::GLenum mode);

    /**
    *  @brief
    *    Get shader program used by the render pass
    *
    *  @return
    *    Shader program (can be null)
    */
    globjects::Program * program() const;

    /**
    *  @brief
    *    Set shader program used by the render pass
    *
    *  @param[in] program
    *    Shader program (can be null)
    *
    *  @notes
    *  - The render pass partially owns the program (increasing its reference counter)
    *  - If both a program and a program pipeline are configured, the program is preferred
    *    during rendering.
    */
    void setProgram(globjects::Program * program);

    /**
    *  @brief
    *    Get program pipeline used by the render pass
    *
    *  @return
    *    Program pipeline (can be null)
    */
    globjects::ProgramPipeline * programPipeline() const;

    /**
    *  @brief
    *    Set program pipeline used by the render pass
    *
    *  @param[in] program
    *    Program pipeline (can be null)
    *
    *  @notes
    *  - The render pass partially owns the program pipeline (increasing its reference counter)
    *  - If both a program and a program pipeline are configured, the program is preferred
    *    during rendering.
    */
    void setProgramPipeline(globjects::ProgramPipeline * programPipeline);

    /**
    *  @brief
    *    Get texture used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *
    *  @return
    *    Texture (can be null)
    */
    globjects::Texture * texture(size_t index) const;

    /**
    *  @brief
    *    Get texture used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *
    *  @return
    *    Texture (can be null)
    */
    globjects::Texture * texture(gl::GLenum activeTextureIndex) const;

    /**
    *  @brief
    *    Set texture used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *  @param[in] texture
    *    Texture (must NOT be null!)
    *
    *  @notes
    *  - To exclude a texture from getting bound active during draw calls,
    *    use removeTexture.
    */
    void setTexture(size_t index, globjects::Texture * texture);

    /**
    *  @brief
    *    Set texture used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *  @param[in] texture
    *    Texture (must NOT be null!)
    *
    *  @notes
    *  - The index is normalized to a size_t, so the texture is also available
    *    using the size_t interface.
    *  - To exclude a texture from getting bound active during draw calls,
    *    use removeTexture.
    */
    void setTexture(gl::GLenum activeTextureIndex, globjects::Texture * texture);

    /**
    *  @brief
    *    Remove texture from being used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *
    *  @return
    *    Texture that was formerly associated with this index (can be null)
    */
    globjects::Texture * removeTexture(size_t index);

    /**
    *  @brief
    *    Remove texture from being used by the render pass
    *
    *  @param[in] index
    *    Texture index (does not need to be continuous)
    *
    *  @return
    *    Texture that was formerly associated with this index (can be null)
    *
    *  @notes
    *  - The index is converted to a size_t, so a texture configured through
    *    the size_t interface is also affected.
    */
    globjects::Texture * removeTexture(gl::GLenum index);

    /**
    *  @brief
    *    Get sampler used by the render pass
    *
    *  @param[in] index
    *    Sampler index (does not need to be continuous)
    *
    *  @return
    *    Sampler (can be null)
    */
    globjects::Sampler * sampler(size_t index) const;

    /**
    *  @brief
    *    Set sampler used by the render pass
    *
    *  @param[in] index
    *    Sampler index (does not need to be continuous)
    *  @param[in] sampler
    *    Sampler (must NOT be null!)
    *
    *  @notes
    *  - Use removeSampler() to remove a sampler from the render pass.
    */
    void setSampler(size_t index, globjects::Sampler * sampler);

    /**
    *  @brief
    *    Remove sampler from being used by the render pass
    *
    *  @param[in] index
    *    Sampler index (does not need to be continuous)
    *
    *  @return
    *    Sampler that was formerly bound to this index (can be null)
    */
    globjects::Sampler * removeSampler(size_t index);

    /**
    *  @brief
    *    Get uniform buffer used by the render pass
    *
    *  @param[in] index
    *    Uniform buffer index (does not need to be continuous)
    *
    *  @return
    *    Uniform buffer (can be null)
    */
    globjects::Buffer * uniformBuffer(size_t index) const;

    /**
    *  @brief
    *    Set uniform buffer used by the render pass
    *
    *  @param[in] index
    *    Uniform buffer index (does not need to be continuous)
    *  @param[in] buffer
    *    Uniform buffer (must NOT be null!)
    *
    *  @notes
    *  - Use removeUniformBuffer() to remove a uniform buffer from the render pass.
    */
    void setUniformBuffer(size_t index, globjects::Buffer * buffer);

    /**
    *  @brief
    *    Remove uniform buffer from being used by the render pass
    *
    *  @param[in] index
    *    Uniform buffer index (does not need to be continuous)
    *
    *  @return
    *    Uniform buffer that was formerly bound to this index (can be null)
    */
    globjects::Buffer * removeUniformBuffer(size_t index);

    /**
    *  @brief
    *    Get atomic counter buffer used by the render pass
    *
    *  @param[in] index
    *    Atomic counter buffer index (does not need to be continuous)
    *
    *  @return
    *    Atomic counter buffer (can be null)
    */
    globjects::Buffer * atomicCounterBuffer(size_t index) const;

    /**
    *  @brief
    *    Set atomic counter buffer used by the render pass
    *
    *  @param[in] index
    *    Atomic counter buffer index (does not need to be continuous)
    *  @param[in] buffer
    *    Atomic counter buffer (must NOT be null!)
    *
    *  @notes
    *  - Use removeAtomicCounterBuffer() to remove an atomic counter buffer from the render pass.
    */
    void setAtomicCounterBuffer(size_t index, globjects::Buffer * buffer);

    /**
    *  @brief
    *    Remove atomic counter buffer from being used by the render pass
    *
    *  @param[in] index
    *    Atomic counter buffer index (does not need to be continuous)
    *
    *  @return
    *    Atomic counter buffer that was formerly bound to this index (can be null)
    */
    globjects::Buffer * removeAtomicCounterBuffer(size_t index);

    /**
    *  @brief
    *    Get shader storage buffer used by the render pass
    *
    *  @param[in] index
    *    Shader storage buffer index (does not need to be continuous)
    *
    *  @return
    *    Shader storage buffer (can be null)
    */
    globjects::Buffer * shaderStorageBuffer(size_t index) const;

    /**
    *  @brief
    *    Set shader storage buffer used by the render pass
    *
    *  @param[in] index
    *    Shader storage buffer index (does not need to be continuous)
    *  @param[in] buffer
    *    Shader storage buffer (must NOT be null!)
    *
    *  @notes
    *  - Use removeShaderStorageBuffer() to remove a shader storage buffer from the render pass.
    */
    void setShaderStorageBuffer(size_t index, globjects::Buffer * buffer);

    /**
    *  @brief
    *    Remove shader storage buffer from being used by the render pass
    *
    *  @param[in] index
    *    Shader storage buffer index (does not need to be continuous)
    *
    *  @return
    *    Shader storage buffer that was formerly bound to this index (can be null)
    */
    globjects::Buffer * removeShaderStorageBuffer(size_t index);

    /**
    *  @brief
    *    Get transform feedback buffer used by the render pass
    *
    *  @param[in] index
    *    Transform feedback buffer index (does not need to be continuous)
    *
    *  @return
    *    Transform feedback buffer (can be null)
    */
    globjects::Buffer * transformFeedbackBuffer(size_t index) const;

    /**
    *  @brief
    *    Set transform feedback buffer used by the render pass
    *
    *  @param[in] index
    *    Transform feedback buffer index (does not need to be continuous)
    *  @param[in] buffer
    *    Transform feedback buffer (must NOT be null!)
    *
    *  @notes
    *  - Use removeTransformFeedbackBuffer() to remove a transform feedback buffer from the render pass.
    */
    void setTransformFeedbackBuffer(size_t index, globjects::Buffer * buffer);

    /**
    *  @brief
    *    Remove transform feedback buffer from being used by the render pass
    *
    *  @param[in] index
    *    Transform feedback buffer index (does not need to be continuous)
    *
    *  @return
    *    Transform feedback buffer that was formerly bound to this index (can be null)
    */
    globjects::Buffer * removeTransformFeedbackBuffer(size_t index);


protected:
    /**
    *  @brief
    *    Bind all configured resources before rendering
    */
    void bindResources() const;


protected:
    globjects::ref_ptr<globjects::State>             m_stateBefore;                 ///< State applied before rendering
    globjects::ref_ptr<globjects::State>             m_stateAfter;                  ///< State applied after rendering
    globjects::ref_ptr<Drawable>                     m_geometry;                    ///< Geometry rendered by the render pass
    globjects::ref_ptr<globjects::Program>           m_program;                     ///< Program used for rendering
    globjects::ref_ptr<globjects::ProgramPipeline>   m_programPipeline;             ///< Program pipeline used for rendering
    globjects::ref_ptr<globjects::TransformFeedback> m_recordTransformFeedback;     ///< Transform feedback object for recording (can be null)
    gl::GLenum                                       m_recordTransformFeedbackMode; ///< Primitive mode for recording transform feedback
    globjects::ref_ptr<globjects::TransformFeedback> m_drawTransformFeedback;       ///< Transform feedback object for playback (can be null)
    gl::GLenum                                       m_drawTransformFeedbackMode;   ///< Primitive mode for playback transform feedback

    std::unordered_map<size_t, globjects::ref_ptr<globjects::Texture>> m_textures;                 /// Collection of all textures associated with this render pass. The key is used as the active texture binding.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Sampler>> m_samplers;                 /// Collection of all samplers associated with this render pass. The key is used as the sampler binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>>  m_uniformBuffers;           /// Collection of all uniform buffers associated with this render pass. The key is used as the uniform buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>>  m_atomicCounterBuffers;     /// Collection of all atomic counter buffers associated with this render pass. The key is used as the atomic counter buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>>  m_shaderStorageBuffers;     /// Collection of all shader storage buffers associated with this render pass. The key is used as the shader storage buffer binding index.
    std::unordered_map<size_t, globjects::ref_ptr<globjects::Buffer>>  m_transformFeedbackBuffers; /// Collection of all transform feedback buffers associated with this render pass. The key is used as the transform feedback buffer binding index.
};


} // namespace gloperate
