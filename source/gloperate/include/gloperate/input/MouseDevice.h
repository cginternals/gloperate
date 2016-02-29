
#pragma once

#include <gloperate/input/AbstractDevice.h>
#include <gloperate/gloperate_api.h>
#include <glm/glm.hpp>

namespace gloperate{

class GLOPERATE_API MouseDevice : public AbstractDevice
{
public:
    /**
     * @brief
     *   Constructor
     *
     * @param[in] descriptor
     *   the device descriptor
     */
    MouseDevice(InputManager * InputManager, const std::string & deviceDescriptor);

    /**
     * @brief
     *   Destructor
     */
    virtual ~MouseDevice();

    /**
     * @brief
     *   Poll the current state of the device and fire appropriate Signals
     */
    virtual void update() override;

    void move(const glm::ivec2 & pos);
    void buttonPress(int button, const glm::ivec2 & pos);
    void buttonRelease(int button, const glm::ivec2 & pos);
    void wheelScroll(const glm::vec2 & delta, const glm::ivec2 & pos);
};

} //namespace gloperate
