
#pragma once


namespace gloperate
{


class Stage;
class Pipeline;
template <typename T>
class Output;
template <typename T>
class ProxyOutput;


//@{
/**
*  @brief
*    Helper template for selecting the output type for pipeline interfaces
*
*  @remarks
*    Internal class used by OutputType.
*
*  @see
*    OutputType
*/
template <typename T, typename StageType>
class OutputTypeSelector
{
};

template <typename T>
class OutputTypeSelector<T, Stage>
{
public:
    using type = Output<T>;
};

template <typename T>
class OutputTypeSelector<T, Pipeline>
{
public:
    using type = ProxyOutput<T>;
};
//@}

//@{
/**
*  @brief
*    Select output type for pipeline interfaces
*
*    Pipeline interfaces can be instanciated for either a Stage or a Pipeline.
*    Depending on this type, the declared outputs need to be either of type
*    Output<T> or ProxyOutput<T>. This helper template selects the appropriate
*    class based on the type of the stage.
*
*    Example usage:
*
*    \code{.cpp}
*        class MyStageInterface:
*        {
*        public:
*            // Type alias for output type
*            template <typename T>
*            using OutputType = gloperate::OutputType<T, StageType>;
*
*        public:
*            // Inputs
*            ...
*
*            // Outputs
*            OutputType<bool> rendered;
*        }
*    \endcode
*/
template <typename T, typename StageType>
class OutputType : public OutputTypeSelector<T, StageType>::type
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Property name
    *  @param[in] parent
    *    Parent stage (must NOT be null!)
    *  @param[in] value
    *    Default value
    */
    OutputType(const std::string & name, Stage * parent, const T & defaultValue = T())
    : OutputTypeSelector<T, StageType>::type(name, parent, defaultValue)
    {
    }

    /**
    *  @brief
    *    Destructor
    */
    virtual ~OutputType()
    {
    }
};
//@}


} // namespace gloperate
