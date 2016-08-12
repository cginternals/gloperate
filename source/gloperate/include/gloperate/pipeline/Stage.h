
#pragma once


#include <vector>
#include <unordered_map>
#include <string>

#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>

// Include Component<Stage> specialization for downstream plugins
#include <gloperate/pipeline/StageComponent.h>


namespace gloperate
{


class Environment;
class AbstractGLContext;
class AbstractSlot;
class AbstractInputSlot;
class AbstractDataSlot;
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
    cppexpose::Signal<AbstractInputSlot *> inputAdded;         ///< Called when an input has been added
    cppexpose::Signal<AbstractInputSlot *> inputRemoved;       ///< Called when an input has been removed
    cppexpose::Signal<AbstractDataSlot *>  parameterAdded;     ///< Called when a parameter has been added
    cppexpose::Signal<AbstractDataSlot *>  parameterRemoved;   ///< Called when a parameter has been removed
    cppexpose::Signal<AbstractDataSlot *>  outputAdded;        ///< Called when an output has been added
    cppexpose::Signal<AbstractDataSlot *>  outputRemoved;      ///< Called when an output has been removed
    cppexpose::Signal<AbstractInputSlot *> proxyOutputAdded;   ///< Called when a proxy output has been added
    cppexpose::Signal<AbstractInputSlot *> proxyOutputRemoved; ///< Called when a proxy output has been removed


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    Stage(Environment * environment, const std::string & name = "Stage");

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
    *    Get gloperate environment
    *
    *  @return
    *    Environment to which the stage belongs (must NOT be null!)
    */
    Environment * environment() const;

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
    *    Check if this stage depends on another stage
    *
    *  @param[in] stage
    *    Stage (must NOT be null!)
    *  @param[in] recursive
    *    If 'true', check recursively, else only direct dependencies are checked
    *
    *  @return
    *    'true' if stage depends on the other stage, else 'false'
    */
    bool requires(const Stage * stage, bool recursive = true) const;

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
    *    Check if stage needs to be processed
    *
    *  @return
    *    'true' if stage needs processing, else 'false'
    *
    *  @remarks
    *    A stage needs to be processed when it has output
    *    data that is marked as required, but is currently
    *    invalid. Also, if a stage is marked as 'alwaysProcess',
    *    it will always return 'true'.
    */
    bool needsProcessing() const;

    /**
    *  @brief
    *    Update the required-state of the stage's input slots
    *
    *  @param[in] slot
    *    Output slot which has changed its required-state
    *
    *  @remarks
    *    By default, all input slots of a stage are marked as required,
    *    if at least one output slot is required. This behavior can
    *    be overridden with the onOutputRequiredChanged method.
    */
    void outputRequiredChanged(AbstractSlot * slot);

    /**
    *  @brief
    *    Update the validity of the stage's output slots
    *
    *  @param[in] slot
    *    Input slot which has changed its value
    *
    *  @remarks
    *    By default, all outputs are invalidated when any input
    *    value of the stage has changed. This behavior can
    *    be overridden with the onInputValueChanged method.
    */
    void inputValueChanged(AbstractSlot * slot);

    /**
    *  @brief
    *    Check if stage is always processed
    *
    *  @return
    *    'true' if stage is always processed, else 'false'
    *
    *  @remarks
    *    A stage that is marked 'alwaysProcess' will be
    *    executed each time its parent pipeline is executed.
    *    This should be used with great care. Usually, only
    *    blit stages should be marked 'alwaysProcess', as
    *    they need to be executed whenever a redraw occurs
    *    to recreate the image even if nothing has changed
    *    in the pipeline. Everything else should be managed
    *    by using data connections.
    */
    bool alwaysProcessed() const;

    /**
    *  @brief
    *    Check if stage is always processed
    *
    *  @param[in] alwaysProcess
    *    'true' if stage is always processed, else 'false'
    *
    *  @see
    *    alwaysProcessed()
    */
    void setAlwaysProcessed(bool alwaysProcess);

    /**
    *  @brief
    *    Invalidate all outputs
    */
    void invalidateOutputs();

    /**
    *  @brief
    *    Get inputs
    *
    *  @return
    *    List of inputs on the stage
    */
    const std::vector<AbstractInputSlot *> & inputs() const;

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
    const AbstractInputSlot * input(const std::string & name) const;

    /**
    *  @brief
    *    Add input
    *
    *  @param[in] input
    *    Input (must NOT null!)
    *  @param[in] ownership
    *    Property ownership
    */
    void addInput(AbstractInputSlot * input, cppexpose::PropertyOwnership ownership);

    /**
    *  @brief
    *    Remove input
    *
    *  @param[in] input
    *    Input (must NOT null!)
    */
    void removeInput(AbstractInputSlot * input);

    /**
    *  @brief
    *    Create a new input on this stage
    *
    *  @param[in] name
    *    Name of the new input
    *
    *  @param[in] defaultValue
    *    Optional default value
    *
    *  @tparam T
    *    Type of the new input
    *
    *  @return
    *    Reference to the newly created Input
    */
    template <typename T>
    Input<T> & createNewInput(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Get parameters
    *
    *  @return
    *    List of parameters on the stage
    */
    const std::vector<AbstractDataSlot *> & parameters() const;

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
    const AbstractDataSlot * parameter(const std::string & name) const;

    /**
    *  @brief
    *    Add parameter
    *
    *  @param[in] parameter
    *    Parameter (must NOT null!)
    *  @param[in] ownership
    *    Property ownership
    */
    void addParameter(AbstractDataSlot * parameter, cppexpose::PropertyOwnership ownership);

    /**
    *  @brief
    *    Remove parameter
    *
    *  @param[in] parameter
    *    Parameter (must NOT null!)
    */
    void removeParameter(AbstractDataSlot * parameter);

    /**
    *  @brief
    *    Create a new parameter on this stage
    *
    *  @param[in] name
    *    Name of the new parameter
    *
    *  @param[in] defaultValue
    *    Optional default value
    *
    *  @tparam T
    *    Type of the new parameter
    *
    *  @return
    *    Reference to the newly created parameter
    */
    template <typename T>
    Parameter<T> & createNewParameter(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Get outputs
    *
    *  @return
    *    List of outputs on the stage
    */
    const std::vector<AbstractDataSlot *> & outputs() const;

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
    const AbstractDataSlot * output(const std::string & name) const;

    /**
    *  @brief
    *    Add output
    *
    *  @param[in] output
    *    Output (must NOT null!)
    *  @param[in] ownership
    *    Property ownership
    */
    void addOutput(AbstractDataSlot * output, cppexpose::PropertyOwnership ownership);

    /**
    *  @brief
    *    Remove output
    *
    *  @param[in] output
    *    Output (must NOT null!)
    */
    void removeOutput(AbstractDataSlot * output);

    /**
    *  @brief
    *    Create a new output on this stage
    *
    *  @param[in] name
    *    Name of the new output
    *
    *  @param[in] defaultValue
    *    Optional default value
    *
    *  @tparam T
    *    Type of the new output
    *
    *  @return
    *    Reference to the newly created output
    */
    template <typename T>
    Output<T> & createNewOutput(const std::string & name, const T & defaultValue = T());

    /**
    *  @brief
    *    Get proxy outputs
    *
    *  @return
    *    List of proxy outputs on the stage
    */
    const std::vector<AbstractInputSlot *> & proxyOutputs() const;

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
    const AbstractInputSlot * proxyOutput(const std::string & name) const;

    /**
    *  @brief
    *    Add proxy output
    *
    *  @param[in] proxyOutput
    *    Proxy output (must NOT null!)
    *  @param[in] ownership
    *    Property ownership
    */
    void addProxyOutput(AbstractInputSlot * proxyOutput, cppexpose::PropertyOwnership ownership);

    /**
    *  @brief
    *    Remove proxy output
    *
    *  @param[in] proxyOutput
    *    Proxy output (must NOT null!)
    */
    void removeProxyOutput(AbstractInputSlot * proxyOutput);

    /**
    *  @brief
    *    Create a new proxy output on this stage
    *
    *  @param[in] name
    *    Name of the new proxy output
    *
    *  @param[in] defaultValue
    *    Optional default value
    *
    *  @tparam T
    *    Type of the new proxy output
    *
    *  @return
    *    Reference to the newly created proxy output
    */
    template <typename T>
    ProxyOutput<T> & createNewProxyOutput(const std::string & name, const T & defaultValue = T());


protected:
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

    /**
    *  @brief
    *    Called when an input value has changed
    *
    *  @param[in] slot
    *    Input slot (either input or parameter)
    *
    *  @remarks
    *    The default implementation invalidates all outputs whenever
    *    an input and parameter has been changed. This method can
    *    be overridden to refine that logic, e.g., invalidate only
    *    certain outputs on certain inputs.
    *
    *    IMPORTANT: Do not make any OpenGL calls in this function,
    *    because there is no OpenGL context active at the time this
    *    function is called. Use it only for invalidating outputs and
    *    implement everything else in onProcess().
    */
    virtual void onInputValueChanged(AbstractSlot * slot);

    /**
    *  @brief
    *    Called when an output value's required-state has changed
    *
    *  @param[in] slot
    *    Output slot (either output or proxy output)
    *
    *  @remarks
    *    The default implementation is to require all input slots
    *    if any output slot is required. This method can be
    *    overridden to refine that logic, e.g., require only
    *    certain inputs on certain required outputs.
    *
    *    IMPORTANT: Do not make any OpenGL calls in this function,
    *    because there is no OpenGL context active at the time this
    *    function is called. Use it only for invalidating outputs and
    *    implement everything else in onProcess().
    */
    virtual void onOutputRequiredChanged(AbstractSlot * slot);


protected:
    Environment * m_environment;    ///< Gloperate environment to which the stage belongs
    bool          m_alwaysProcess;  ///< Is the stage always processed?

    std::vector<AbstractInputSlot *>                     m_inputs;          ///< List of inputs
    std::unordered_map<std::string, AbstractInputSlot *> m_inputsMap;       ///< Map of names and inputs
    std::vector<AbstractDataSlot *>                      m_parameters;      ///< List of parameters
    std::unordered_map<std::string, AbstractDataSlot *>  m_parametersMap;   ///< Map of names and parameters
    std::vector<AbstractDataSlot *>                      m_outputs;         ///< List of outputs
    std::unordered_map<std::string, AbstractDataSlot *>  m_outputsMap;      ///< Map of names and outputs
    std::vector<AbstractInputSlot *>                     m_proxyOutputs;    ///< List of proxy outputs
    std::unordered_map<std::string, AbstractInputSlot *> m_proxyOutputsMap; ///< Map of names and proxy outputs

    std::vector<AbstractSlot *>                          m_dynamicSlots;    ///< List of slots created dynamically
};


} // namespace gloperate

#include <gloperate/pipeline/Stage.inl>
