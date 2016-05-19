
#pragma once


#include <vector>
#include <unordered_map>
#include <string>

#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class ViewerContext;
class AbstractGLContext;
class AbstractInput;
class AbstractParameter;
class AbstractOutput;
class AbstractProxyOutput;
class Pipeline;

template <typename T>
class Input;

template <typename T>
class Parameter;

template <typename T>
class Output;

template <typename T>
class ProxyOutput;


/**
*  @brief
*    Base class for stages
*
*    A stage describes a functional entity that produces a certain set of outputs
*    and depends on a certain set of inputs. It can either be part of a pipeline
*    or stand on its own.
*/
class GLOPERATE_API Stage : public cppexpose::Object
{
friend class AbstractInput;
friend class AbstractParameter;
friend class AbstractOutput;
friend class AbstractProxyOutput;


public:
    // Import data types into local namespace
    template <typename T>
    using Input = gloperate::Input<T>;

    template <typename T>
    using Parameter = gloperate::Parameter<T>;

    template <typename T>
    using Output = gloperate::Output<T>;

    template <typename T>
    using ProxyOutput = gloperate::ProxyOutput<T>;


public:
    cppexpose::Signal<AbstractInput *>       inputAdded;         ///< Called when an input has been added
    cppexpose::Signal<AbstractInput *>       inputRemoved;       ///< Called when an input has been removed
    cppexpose::Signal<AbstractParameter *>   parameterAdded;     ///< Called when a parameter has been added
    cppexpose::Signal<AbstractParameter *>   parameterRemoved;   ///< Called when a parameter has been removed
    cppexpose::Signal<AbstractOutput *>      outputAdded;        ///< Called when an output has been added
    cppexpose::Signal<AbstractOutput *>      outputRemoved;      ///< Called when an output has been removed
    cppexpose::Signal<AbstractProxyOutput *> proxyOutputAdded;   ///< Called when a proxy output has been added
    cppexpose::Signal<AbstractProxyOutput *> proxyOutputRemoved; ///< Called when a proxy output has been removed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    Stage(ViewerContext * viewerContext, const std::string & name = "Stage", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Stage();

    /**
    *  @brief
    *    Check if stage is a pipeline
    *
    *  @return
    *    'true' if stage is a pipeline, else 'false'
    */
    virtual bool isPipeline() const;

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context to which the stage belongs (must NOT be null!)
    */
    ViewerContext * viewerContext() const;

    /**
    *  @brief
    *    Get parent pipeline
    *
    *  @return
    *    Pipeline to which the stage belongs (can be null)
    */
    Pipeline * parentPipeline() const;

    /**
    *  @brief
    *    Set parent pipeline
    *
    *  @param[in] parent
    *    Parent pipeline
    *
    *  @remarks
    *    Sets up the stage to be owned by the specified pipeline.
    *    Will set both the owner pipeline and the parent property
    *    to the pipeline and transfer ownership to the pipeline.
    *    Does only work if the stage did not have a parent
    *    before. Otherwise, the function will just return.
    */
    void transferStage(Pipeline * parent);

    /**
    *  @brief
    *    Initialize in OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextInit
    */
    void initContext(AbstractGLContext * context);

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextDeinit
    */
    void deinitContext(AbstractGLContext * context);

    /**
    *  @brief
    *    Process stage
    *
    *  @param[in] context
    *    OpenGL context that is current (must NOT null!)
    *
    *  @see onProcess
    */
    void process(AbstractGLContext * context);

    /**
    *  @brief
    *    Get inputs
    *
    *  @return
    *    List of inputs on the stage
    */
    const std::vector<AbstractInput *> & inputs() const;

    /**
    *  @brief
    *    Get input by name
    *
    *  @param[in] name
    *    Name of input
    *
    *  @return
    *    Input (can be null)
    */
    const AbstractInput * input(const std::string & name) const;

    /**
    *  @brief
    *    Get parameters
    *
    *  @return
    *    List of parameters on the stage
    */
    const std::vector<AbstractParameter *> & parameters() const;

    /**
    *  @brief
    *    Get parameter by name
    *
    *  @param[in] name
    *    Name of parameter
    *
    *  @return
    *    Parameter (can be null)
    */
    const AbstractParameter * parameter(const std::string & name) const;

    /**
    *  @brief
    *    Get outputs
    *
    *  @return
    *    List of outputs on the stage
    */
    const std::vector<AbstractOutput *> & outputs() const;

    /**
    *  @brief
    *    Get output by name
    *
    *  @param[in] name
    *    Name of output
    *
    *  @return
    *    Output (can be null)
    */
    const AbstractOutput * output(const std::string & name) const;

    /**
    *  @brief
    *    Get proxy outputs
    *
    *  @return
    *    List of proxy outputs on the stage
    */
    const std::vector<AbstractProxyOutput *> & proxyOutputs() const;

    /**
    *  @brief
    *    Get proxy output by name
    *
    *  @param[in] name
    *    Name of proxy output
    *
    *  @return
    *    Proxy output (can be null)
    */
    const AbstractProxyOutput * proxyOutput(const std::string & name) const;


protected:
    /**
    *  @brief
    *    Register input
    *
    *  @param[in] input
    *    Input (must NOT null!)
    *
    *  @remarks
    *    Although inputs and outputs are already registered at the stage
    *    as properties, we keep a separate list of them to simplify
    *    accessing inputs and outputs directly and without type casts.
    */
    void registerInput(AbstractInput * input);

    /**
    *  @brief
    *    Unregister input
    *
    *  @param[in] input
    *    Input (must NOT null!)
    */
    void unregisterInput(AbstractInput * input);

    /**
    *  @brief
    *    Register parameter
    *
    *  @param[in] parameter
    *    Parameter (must NOT null!)
    *
    *  @see
    *    registerInput
    */
    void registerParameter(AbstractParameter * parameter);

    /**
    *  @brief
    *    Unregister parameter
    *
    *  @param[in] parameter
    *    Parameter (must NOT null!)
    */
    void unregisterParameter(AbstractParameter * parameter);

    /**
    *  @brief
    *    Register output
    *
    *  @param[in] output
    *    Output (must NOT null!)
    *
    *  @see
    *    registerInput
    */
    void registerOutput(AbstractOutput * output);

    /**
    *  @brief
    *    Unregister output
    *
    *  @param[in] output
    *    Output (must NOT null!)
    */
    void unregisterOutput(AbstractOutput * output);

    /**
    *  @brief
    *    Register proxy output
    *
    *  @param[in] proxyOutput
    *    Proxy output (must NOT null!)
    *
    *  @see
    *    registerInput
    */
    void registerProxyOutput(AbstractProxyOutput * proxyOutput);

    /**
    *  @brief
    *    Unregister proxy output
    *
    *  @param[in] proxyOutput
    *    Proxy output (must NOT null!)
    */
    void unregisterProxyOutput(AbstractProxyOutput * proxyOutput);

    /**
    *  @brief
    *    Initialize in OpenGL context
    *
    *    This function is called whenever the OpenGL context has changed.
    *    This occurs not only when the viewer is created, but can also happen
    *    for example when switching between window and fullscreen mode.
    *    Therefore, the object must be aware that onContextInit() and
    *    onContextDeinit() can be called multiple times over its lifetime and
    *    must be able to create/restore all necessary OpenGL objects, such as
    *    textures and geometries, in this function.
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    */
    virtual void onContextInit(AbstractGLContext * context);

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *    This function is called when the OpenGL context is destroyed.
    *    The object must release its OpenGL objects at this point.
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextInit()
    */
    virtual void onContextDeinit(AbstractGLContext * context);

    /**
    *  @brief
    *    Process stage
    *
    *    This function is called when the stage is executed.
    *    At this point, the stage is expected to process its
    *    inputs and create its output data.
    *
    *  @param[in] context
    *    OpenGL context that is current (must NOT null!)
    *
    *  @remarks
    *    The provided OpenGL context is already made current by
    *    the caller of this function, i.e., the viewer or parent
    *    pipeline. Therefore, the stage does not need to use
    *    the context pointer in most cases. However, if the stage
    *    manages its own context, it needs to rebind the former
    *    context at the end of its execution.
    */
    virtual void onProcess(AbstractGLContext * context);


protected:
    ViewerContext * m_viewerContext;  ///< Viewer context to which the stage belongs
    Pipeline      * m_parentPipeline; ///< Pipeline to which the stage belongs (can be null)

    std::vector<AbstractInput *>                           m_inputs;          ///< List of inputs
    std::unordered_map<std::string, AbstractInput *>       m_inputsMap;       ///< Map of names and inputs
    std::vector<AbstractParameter *>                       m_parameters;      ///< List of parameters
    std::unordered_map<std::string, AbstractParameter *>   m_parametersMap;   ///< Map of names and parameters
    std::vector<AbstractOutput *>                          m_outputs;         ///< List of outputs
    std::unordered_map<std::string, AbstractOutput *>      m_outputsMap;      ///< Map of names and outputs
    std::vector<AbstractProxyOutput *>                     m_proxyOutputs;    ///< List of proxy outputs
    std::unordered_map<std::string, AbstractProxyOutput *> m_proxyOutputsMap; ///< Map of names and proxy outputs
};


} // namespace gloperate
