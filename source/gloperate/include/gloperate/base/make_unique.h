
#pragma once


#include <memory>


namespace gloperate
{


// copied from libc++ (MIT-licensed)

template<class _Tp>
struct __unique_if
{
    typedef std::unique_ptr<_Tp> __unique_single;
};

template<class _Tp>
struct __unique_if<_Tp[]>
{
    typedef std::unique_ptr<_Tp[]> __unique_array_unknown_bound;
};

template<class _Tp, size_t _Np>
struct __unique_if<_Tp[_Np]>
{
    typedef void __unique_array_known_bound;
};

template<class _Tp, class... _Args>
typename __unique_if<_Tp>::__unique_single
make_unique(_Args&&... __args);

template<class _Tp>
typename __unique_if<_Tp>::__unique_array_unknown_bound
make_unique(size_t __n);

template<class _Tp, class... _Args>
typename __unique_if<_Tp>::__unique_array_known_bound
make_unique(_Args&&...) = delete;


} // namespace gloperate


#include <gloperate/base/make_unique.inl>
