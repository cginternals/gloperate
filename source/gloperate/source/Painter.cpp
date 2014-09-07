#include <gloperate/Painter.h>


namespace gloperate
{


Painter::Painter()
{
}

Painter::~Painter()
{
}

void Painter::initialize()
{
    onInitialize();
}

void Painter::resize(const Viewport & viewport)
{
    onResize(viewport);
}

void Painter::paint()
{
    onPaint();
}


} // gloperate
