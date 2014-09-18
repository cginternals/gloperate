#include <gloperate/pipelines/PerformanceMonitor.h>

#include <glbinding/gl/enum.h>

#include <gloperate/pipelines/Stage.h>

#include <globjects/Query.h>

namespace gloperate 
{

Timer::Timer()
: m_reset(false)
{
    m_query = new globjects::Query();
}

void Timer::begin()
{
    m_startTime = std::chrono::high_resolution_clock::now();
    m_query->begin(gl::GL_TIME_ELAPSED);
    m_reset = false;
}

void Timer::end()
{
    m_query->end(gl::GL_TIME_ELAPSED);
    m_endTime = std::chrono::high_resolution_clock::now();
}

void Timer::reset()
{
    end();
    m_reset = true;
}

Timer::Duration Timer::cpuTime() const
{
    if (m_reset)
        return Duration::zero();
    return std::chrono::duration_cast<Duration>(m_endTime - m_startTime);
}

Timer::Duration Timer::gpuTime() const
{
    if (m_reset)
        return Duration::zero();
    return Duration(m_query->waitAndGet64(gl::GL_QUERY_RESULT, std::chrono::duration<int, std::milli>(100)));
}


ResultQueue::ResultQueue()
: m_maxElementCount(0)
{
}

ResultQueue::ResultQueue(const std::vector<std::string>& names)
: m_names(names)
, m_maxElementCount(100)
{
}

void ResultQueue::push(const Element& element)
{
    m_elements.push_back(element);
    if (m_elements.size()>m_maxElementCount)
        pop(1);
}

void ResultQueue::pop(unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        m_elements.pop_front();
}

double ResultQueue::at(unsigned element, unsigned sample) const
{
    return m_elements.at(element).at(sample);
}

unsigned ResultQueue::size() const
{
    return m_elements.size();
}

const std::deque<ResultQueue::Element>& ResultQueue::elements() const
{
    return m_elements;
}

const std::vector<std::string>& ResultQueue::names() const
{
    return m_names;
}

unsigned ResultQueue::nameCount() const
{
    return m_names.size();
}

void ResultQueue::clear()
{
    m_elements.clear();
}

void ResultQueue::setMaxElementCount(unsigned int n)
{
    m_maxElementCount = n;
}



PerformanceMonitor::PerformanceMonitor()
: m_fps(0)
, m_started(false)
{
}

PerformanceMonitor::~PerformanceMonitor()
{
    deleteTimers();
}

ResultQueue& PerformanceMonitor::results()
{
    return m_results;
}

double PerformanceMonitor::fps() const
{
    return m_fps;
}

void PerformanceMonitor::deleteTimers()
{
    for (Timer* timer: m_timers)
    {
        delete timer;
    }
}

void PerformanceMonitor::monitorStages(const std::vector<Stage*>& pipeline)
{
    deleteTimers();

    m_pipeline = pipeline;

    std::vector<std::string> names;
    int index = 0;
    for (const Stage* stage: m_pipeline)
    {
        names.push_back(stage->name());
        m_timers.push_back(new Timer());
        m_indices[stage] = index++;
    }

    m_results = ResultQueue(names);
}

void PerformanceMonitor::begin()
{
    if (!m_started)
    {
        m_time = std::chrono::high_resolution_clock::now();
        m_started = true;
        return;
    }

    Timer::Duration frameDuration = std::chrono::duration_cast<Timer::Duration>(std::chrono::high_resolution_clock::now() - m_time);
    double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(frameDuration).count();

    m_time = std::chrono::high_resolution_clock::now();

    m_frames.push_back(seconds);
    if (m_frames.size()>100)
        m_frames.pop_front();

    double total = 0;
    for (double d: m_frames)
    {
        total += d;
    }

    m_fps = m_frames.size() / total;
}

void PerformanceMonitor::end()
{
    ResultQueue::Element element;

    for (unsigned int i = 0; i<m_pipeline.size(); ++i)
    {
        const Stage* stage = m_pipeline[i];
        Timer* timer = m_timers[i];

        Timer::Duration duration = stage->mode()==Stage::CPU ? timer->cpuTime() : timer->gpuTime();

        static const double ms = (double)std::chrono::milliseconds::period::num / (double)std::chrono::milliseconds::period::den;

        double milliseconds = (double)duration.count() * (double)Timer::Duration::period::num / (double)Timer::Duration::period::den / ms ;

        element.push_back(milliseconds);
    }

    m_results.push(element);
}

void PerformanceMonitor::beginStage(Stage* stage)
{
    if (m_indices.find(stage) == m_indices.end()) return;

    m_timers[m_indices[stage]]->begin();
}

void PerformanceMonitor::endStage(Stage* stage)
{
    if (m_indices.find(stage) == m_indices.end()) return;

    m_timers[m_indices[stage]]->end();
}

void PerformanceMonitor::resetStage(Stage* stage)
{
    if (m_indices.find(stage) == m_indices.end()) return;

    m_timers[m_indices[stage]]->reset();
}

} // namespace gloperate
