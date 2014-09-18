#pragma once

#include <unordered_map>
#include <vector>
#include <deque>
#include <chrono>

#include <globjects/base/ref_ptr.h>

#include <globjects/Query.h>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class Stage;

// ToDo: MOOEP -> Use glow timer!
class GLOPERATE_API Timer
{
public:
    typedef std::chrono::nanoseconds Duration;

    Timer();

    void begin();
    void end();
    void reset();

    Duration cpuTime() const;
    Duration gpuTime() const;
protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_endTime;
    globjects::ref_ptr<globjects::Query> m_query;
    bool m_reset;
};

class GLOPERATE_API ResultQueue
{
public:
    typedef std::vector<double> Element;

    ResultQueue();
    ResultQueue(const std::vector<std::string>& names);

    void push(const Element& element);
    void pop(unsigned int n = 1);

    double at(unsigned element, unsigned sample) const;

    unsigned size() const;
    const std::deque<Element>& elements() const;

    const std::vector<std::string>& names() const;
    unsigned nameCount() const;

    void clear();

    void setMaxElementCount(unsigned int n);
protected:
    std::deque<Element> m_elements;
    std::vector<std::string> m_names;
    unsigned int m_maxElementCount;
};

class GLOPERATE_API PerformanceMonitor
{
public:
    PerformanceMonitor();
    virtual ~PerformanceMonitor();

    void monitorStages(const std::vector<Stage*>& pipeline);

    void begin();
    void end();

    void beginStage(Stage* stage);
    void endStage(Stage* stage);
    void resetStage(Stage* stage);

    ResultQueue& results();

    double fps() const;
protected:
    ResultQueue m_results;
    std::vector<Stage*> m_pipeline;
    std::unordered_map<const Stage*, unsigned> m_indices;
    std::vector<Timer*> m_timers;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_time;
    std::deque<double> m_frames;
    double m_fps;
    bool m_started;

    void deleteTimers();
};

} // namespace gloperate
