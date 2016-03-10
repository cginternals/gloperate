
#pragma once


#include <gloperate/input/AbstractEventConsumer.h>


namespace gloperate
{


class GLOPERATE_API PrintLineConsumer : public AbstractEventConsumer
{
public:
    PrintLineConsumer(InputManager * inputManager);
    virtual ~PrintLineConsumer();

    virtual void onEvent(InputEvent * event) override;
};


} // namespace gloperate
