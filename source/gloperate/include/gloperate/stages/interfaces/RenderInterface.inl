
#pragma once


namespace gloperate
{


template <typename StageType>
RenderInterface<StageType>::RenderInterface(Stage * stage)
: deviceViewport ("deviceViewport", stage)
, virtualViewport("virtualViewport", stage)
, backgroundColor("backgroundColor", stage)
, frameCounter   ("frameCounter", stage)
, timeDelta      ("timeDelta", stage)
, targetFBO      ("targetFBO", stage)
, rendered       ("rendered", stage)
{
}

template <typename StageType>
RenderInterface<StageType>::~RenderInterface()
{
}


} // namespace gloperate
