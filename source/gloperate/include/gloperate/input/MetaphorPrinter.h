
#pragma once

#include <gloperate/input/Metaphor.h>
#include <gloperate/input/InputEvent.h>
#include <gloperate/gloperate_api.h>


namespace gloperate{


class GLOPERATE_API MetaphorPrinter
{
public:
    MetaphorPrinter();

    void print(InputEvent * event);

    Metaphor<MetaphorPrinter> m_metaphor;

};

} // namespace gloperate
