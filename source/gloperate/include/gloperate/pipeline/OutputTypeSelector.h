
#pragma once


namespace gloperate
{


class Stage;
class Pipeline;
template <typename T>
class Output;
template <typename T>
class ProxyOutput;


template <typename T, typename StageType>
class OutputTypeSelector
{
    static_assert(false, "Use either Stage or Pipeline as StageType parameter.");
};

template <typename T, Stage>
class OutputTypeSelector
{
    using type = Output<T>;
};

template <typename T, Pipeline>
class OutputTypeSelector
{
    using type = ProxyOutput<T>;
};


} // namespace gloperate
