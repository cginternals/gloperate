
#pragma once


namespace gloperate
{


template <typename T>
VectorSelectionStage<T>::VectorSelectionStage(Environment * environment, const std::string & name)
: Stage(environment, "VectorSelectionStage", name)
, elements("elements", this)
, index("index", this, 0)
, defaultValue("defaultValue", this, T())
, element("element", this, defaultValue.value())
{
}


template <typename T>
void VectorSelectionStage<T>::onProcess()
{
    const auto i = index.value();

    if (elements.value() == nullptr || i < 0 || i >= elements.value()->size())
    {
        element.setValue(defaultValue.value());

        return;
    }

    element.setValue(elements.value()->at(i));
}


} // namespace gloperate
