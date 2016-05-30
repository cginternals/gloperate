
#pragma once


#include <gloperate/pipeline/Data.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/PipelineEvent.h>


namespace gloperate
{


template <typename T, typename BASE>
Data<T, BASE>::Data(const T & value)
: cppexpose::DirectValue<T, BASE>(value)
{
}

template <typename T, typename BASE>
Data<T, BASE>::~Data()
{
}

template <typename T, typename BASE>
T & Data<T, BASE>::operator*()
{
    return *this->ptr();
}

template <typename T, typename BASE>
const T & Data<T, BASE>::operator*() const
{
    return *this->ptr();
}

template <typename T, typename BASE>
T * Data<T, BASE>::operator->()
{
    return this->ptr();
}

template <typename T, typename BASE>
const T * Data<T, BASE>::operator->() const
{
    return this->ptr();
}

template <typename T, typename BASE>
bool Data<T, BASE>::isGroup() const
{
    return false;
}

template <typename T, typename BASE>
void Data<T, BASE>::onValueChanged(const T & value)
{
    this->valueChanged(value);

    if (Stage * stage = this->parentStage())
    {
        stage->promotePipelineEvent(
            PipelineEvent(PipelineEvent::ValueChanged, stage, this)
        );
    }
}


} // namespace gloperate
