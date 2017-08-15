
#include <cppassist/memory/make_unique.h>


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
    return cppassist::make_unique<Type>(environment);
}


template <class Type>
StageComponent<Type>::StageComponent()
{
}

template <class Type>
StageComponent<Type>::~StageComponent()
{
}

template <class Type>
std::unique_ptr<gloperate::Stage> StageComponent<Type>::createInstance(gloperate::Environment * environment)
{
    return cppassist::make_unique<Type>(environment);
}

template <class Type>
std::unique_ptr<gloperate::Stage> StageComponent<Type>::createInstance(gloperate::Environment * environment, const std::string & name)
{
    return cppassist::make_unique<Type>(environment, name);
}


} // namespace gloperate
