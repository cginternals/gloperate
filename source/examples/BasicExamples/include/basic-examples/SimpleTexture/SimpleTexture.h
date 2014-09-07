#pragma once


#include <globjects-base/ref_ptr.h>
#include <globjects/Texture.h>
#include <gloperate/util/ScreenAlignedQuad.h>
#include <gloperate/Painter.h>
#include <basic-examples/basic_examples_api.h>


class BASIC_EXAMPLES_API SimpleTexture : public gloperate::Painter
{


public:
    SimpleTexture();
    virtual ~SimpleTexture();


protected:
    virtual void onInitialize();
    virtual void onResize(const gloperate::Viewport & viewport);
    virtual void onPaint();


protected:
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    glo::ref_ptr<glo::Texture>                 m_texture;
    glo::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;


};
