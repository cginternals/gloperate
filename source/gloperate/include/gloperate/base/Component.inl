
#include <gloperate/base/make_unique.h>


namespace gloperate
{


template <class BaseType, class Type>
Component<BaseType, Type>::Component()
{
}

template <class BaseType, class Type>
Component<BaseType, Type>::~Component()
{
}

template <class BaseType, class Type>
std::unique_ptr<BaseType> Component<BaseType, Type>::createInstance(gloperate::Environment * environment)
{
    return gloperate::make_unique<Type>(environment);
}


template <class Type>
Component<gloperate::Stage, Type>::Component()
{
}

template <class Type>
Component<gloperate::Stage, Type>::~Component()
{
}

template <class Type>
gloperate::Stage * Component<gloperate::Stage, Type>::createInstance(gloperate::Environment * environment)
{
    return new Type(environment);
}

template <class Type>
gloperate::Stage * Component<gloperate::Stage, Type>::createInstance(gloperate::Environment * environment, const std::string & name)
{
    return new Type(environment, name);
}


} // namespace gloperate
