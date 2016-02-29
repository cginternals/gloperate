
#pragma once

#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/gloperate_api.h>

namespace gloperate{

class PrintLineConsumer : public AbstractEventConsumer
{
public:
    PrintLineConsumer(InputManager * inputManager);

    virtual ~PrintLineConsumer();

    virtual void onEvent(InputEvent * event) override;
};

} //namespace gloperate
