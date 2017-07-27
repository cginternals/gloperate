
#pragma once


#include <gloperate/pipeline/Slot.h>


namespace gloperate
{


template <typename T>
class Slot;


} // namespace gloperate


namespace
{


template <typename... Types>
struct isOfAnyTypeHelper;

template <typename T, typename... Types>
struct isOfAnyTypeHelper<T, Types...>
{
    static bool value(const gloperate::AbstractSlot * slot)
    {
        return isOfAnyTypeHelper<T>::value(slot) || isOfAnyTypeHelper<Types...>::value(slot);
    }
};

template <typename T>
struct isOfAnyTypeHelper<T>
{
    static bool value(const gloperate::AbstractSlot * slot)
    {
        return dynamic_cast<const gloperate::Slot<T> *>(slot) != nullptr;
    }
};

template <>
struct isOfAnyTypeHelper<>
{
    static bool value(const gloperate::AbstractSlot * /*slot*/)
    {
        return false;
    }
};


} // namespace


namespace gloperate
{


template <typename... Types>
bool AbstractSlot::isOfAnyType() const
{
    return isOfAnyTypeHelper<Types...>::value(this);
}


} // namespace gloperate
