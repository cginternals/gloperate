
#pragma once


#include <unordered_map>
#include <vector>

#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


    class Stage;
    class Canvas;


    /**
    *  @brief
    *    Tool to export measurement data from a canvas
    */
    class GLOPERATE_API MeasurementExporter
    {

    public:
        /**
        *  @brief
        *    Constructor
        */
        MeasurementExporter(Canvas* canvas);

        /**
        *  @brief
        *    Destructor
        */
        ~MeasurementExporter() = default;

        void startMeasuring();
        void stopMeasuring();


    protected:

        void receiveValue(Stage* stage, int cycle, uint64_t cpu_time, uint64_t gpu_time);
        void writeData();

        Canvas* m_canvas;
        cppexpose::ScopedConnection m_connection;
        std::unordered_map<Stage*, std::vector<std::pair<uint64_t, uint64_t>>> m_data;
        std::vector<Stage*> m_stages;
    };


} // namespace gloperate
