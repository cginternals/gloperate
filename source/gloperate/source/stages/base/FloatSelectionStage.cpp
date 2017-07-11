
#include <gloperate/stages/base/FloatSelectionStage.h>

#include <algorithm>


namespace gloperate
{


CPPEXPOSE_COMPONENT(FloatSelectionStage, gloperate::Stage)


FloatSelectionStage::FloatSelectionStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, "FloatSelectionStage", name)
, index("index", this, 0)
, value("value", this, 0.0f)
{
    inputAdded.connect([this](AbstractSlot * slot) {
        auto floatInput = dynamic_cast<Input<float> *>(slot);

        if (floatInput)
        {
            m_floatInputs.push_back(floatInput);
        }
    });

    inputRemoved.connect([this](AbstractSlot * slot) {
        const auto it = std::find(m_floatInputs.begin(), m_floatInputs.end(), static_cast<Input<float> *>(slot));

        if (it == m_floatInputs.end())
        {
            return;
        }

        m_floatInputs.erase(it);
    });
}

FloatSelectionStage::~FloatSelectionStage()
{
}

void FloatSelectionStage::onProcess()
{
    if (m_floatInputs.size() <= *index)
    {
        value.setValue(0.0f);

        return;
    }

    value.setValue(**m_floatInputs.at(*index));
}


} // namespace gloperate
