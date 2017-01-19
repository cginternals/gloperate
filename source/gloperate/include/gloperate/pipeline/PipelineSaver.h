#pragma once

#include <string>
#include <functional>

namespace gloperate{

    class Stage;
    class AbstractSlot;

class PipelineSaver {
public:
    PipelineSaver(const Stage * toSave);

    void save(const std::string& filename);

protected:
    void saveStage(const Stage * stage, std::function<void(const std::string &, uint)> writer, uint level) const;
    std::string getSourcePath(const AbstractSlot * slot) const;
    const Stage * m_toSave;
};

}
