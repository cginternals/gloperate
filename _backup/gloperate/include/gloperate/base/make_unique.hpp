
#pragma once


#include <memory>


namespace gloperate
{


/** 
 * Replacement for C++14 implementation 
 * as long as not all compilers support it. 
 * Prefer over std::unique_ptr<Type>{new Type}
 * \see http://herbsutter.com/gotw/_102/
 */
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


} // namespace gloperate
