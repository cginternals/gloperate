
#pragma once


#include <gloperate/base/make_unique.h>


namespace gloperate
{


template<class _Tp, class... _Args>
typename __unique_if<_Tp>::__unique_single
make_unique(_Args&&... __args)
{
    return std::unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...));
}

template<class _Tp>
typename __unique_if<_Tp>::__unique_array_unknown_bound
make_unique(size_t __n)
{
    typedef typename std::remove_extent<_Tp>::type _Up;
    return std::unique_ptr<_Tp>(new _Up[__n]());
}


} // namespace gloperate
