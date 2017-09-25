
#include <gloperate/tools/MeasurementExporter.h>

#include <fstream>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/base/Canvas.h>


namespace gloperate
{


MeasurementExporter::MeasurementExporter(Canvas* canvas)
: m_canvas(canvas)
{
}

void MeasurementExporter::startMeasuring()
{
    if (!m_canvas)
        return;

    m_connection = m_canvas->receiveMeasurementValue.connect(this, &MeasurementExporter::receiveValue);
    m_canvas->setMeasurement(true);
}

void MeasurementExporter::stopMeasuring()
{
    m_canvas->setMeasurement(false);
    writeData();
    m_data.clear();
    m_stages.clear();
    m_connection = cppexpose::ScopedConnection();
}

void MeasurementExporter::receiveValue(Stage * stage, int cycle, uint64_t cpu_time, uint64_t gpu_time)
{
    if (m_data[stage].size() >= 1000)
        writeData();

    m_data[stage].push_back({ cpu_time, gpu_time });
}

void MeasurementExporter::writeData()
{
    std::ofstream file;
    file.open("temp3782099.csv", std::ios::out | std::ios::app);


    if (m_stages.empty())
    {
        for (auto& each : m_data)
        {
            m_stages.push_back(each.first);
        }

        //write header
        for (auto stage : m_stages)
        {
            file << stage->name() + "_CPU" << ", " << stage->name() + "_GPU" << ", ";
        }
        file << "\n";
    }

    //write content
    bool end = false;
    for (int i = 0; i < 1000 && !end; ++i)
    {
        for (auto stage : m_stages)
        {
            if (m_data[stage].size() <= i)
            {
                end = true;
                break;
            }

            file << m_data[stage][i].first << ", " << m_data[stage][i].second << ", ";
        }
        file << "\n";
    }
    file.close();

    //clear data
    for (auto& each : m_data)
    {
        each.second.clear();
    }
}



} //namespace gloperate