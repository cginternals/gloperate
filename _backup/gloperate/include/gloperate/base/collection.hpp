
#pragma once


#include <utility>
#include <vector>
#include <unordered_map>


namespace collection
{


template <typename InputIterator, typename Mapper>
auto collect(InputIterator first, InputIterator last, Mapper mapper) -> std::vector<decltype(mapper(std::declval<typename InputIterator::value_type>()))>
{
    std::vector<decltype(mapper(std::declval<typename InputIterator::value_type>()))> result;
    while (first!=last)
    {
        result.push_back(mapper(*first));
        ++first;
    }
    return result;
}

template <typename Collection, typename Mapper>
auto collect(const Collection & collection, Mapper mapper) -> decltype(collect(collection.begin(), collection.end(), mapper))
{
    return collect(collection.begin(), collection.end(), mapper);
}

template <typename InputIterator, typename T = typename InputIterator::value_type, typename R = typename T::value_type>
std::vector<R> flatten(InputIterator first, InputIterator last)
{
    std::vector<R> result;
    while (first!=last)
    {
        for (auto begin = first->begin(); begin != first->end(); ++begin)
        {
            result.push_back(*begin);
        }
        ++first;
    }
    return result;
}

template <typename Collection>
auto flatten(const Collection & collection) -> decltype(flatten(collection.begin(), collection.end()))
{
    return flatten(collection.begin(), collection.end());
}

template <typename InputIterator, typename Grouper>
auto groupBy(InputIterator first, InputIterator last, Grouper grouper) -> std::unordered_map<typename std::decay<decltype(grouper(std::declval<typename InputIterator::value_type>()))>::type, std::vector<typename InputIterator::value_type>>
{
    std::unordered_map<typename std::decay<decltype(grouper(std::declval<typename InputIterator::value_type>()))>::type, std::vector<typename InputIterator::value_type>> result;
    while (first!=last)
    {
        result[grouper(*first)].push_back(*first);

        ++first;
    }
    return result;
}

template <typename Collection, typename Grouper>
auto groupBy(const Collection & collection, Grouper grouper) -> decltype(groupBy(collection.begin(), collection.end(), grouper))
{
    return groupBy(collection.begin(), collection.end(), grouper);
}

template <typename InputIterator, typename Selector, typename T = typename InputIterator::value_type>
std::vector<T> select(InputIterator first, InputIterator last, Selector selector)
{
    std::vector<T> result;
    while (first!=last)
    {
        if (selector(*first))
            result.push_back(*first);
        ++first;
    }
    return result;
}

template <typename Collection, typename Selector>
auto select(const Collection & collection, Selector selector) -> decltype(select(collection.begin(), collection.end(), selector))
{
    return select(collection.begin(), collection.end(), selector);
}

template <typename InputIterator, typename Rejecter, typename T = typename InputIterator::value_type>
std::vector<T> reject(InputIterator first, InputIterator last, Rejecter rejecter)
{
    std::vector<T> result;
    while (first!=last)
    {
        if (!rejecter(*first))
            result.push_back(*first);
        ++first;
    }
    return result;
}

template <typename Collection, typename Rejecter>
auto reject(const Collection & collection, Rejecter rejecter) -> decltype(reject(collection.begin(), collection.end(), rejecter))
{
    return reject(collection.begin(), collection.end(), rejecter);
}

template <typename InputIterator, typename Detector, typename Default, typename T = typename InputIterator::value_type>
T detect(InputIterator first, InputIterator last, Detector detector, Default d)
{
    while (first!=last)
    {
        if (detector(*first))
            return *first;
        ++first;
    }

    return d;
}

template <typename Collection, typename Detector, typename Default>
auto detect(const Collection & collection, Detector detector, Default d) -> decltype(detect(collection.begin(), collection.end(), detector, d))
{
    return detect(collection.begin(), collection.end(), detector, d);
}


} // namespace collection
