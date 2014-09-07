#pragma once


#include <globjects-base/ref_ptr.h>
#include <globjects/Texture.h>
#include <gloperate/util/ScreenAlignedQuad.h>
#include <gloperate-qt/QtOpenGLWindow.h>


class ExampleWindow : public gloperate_qt::QtOpenGLWindow
{


public:
    ExampleWindow();
    ~ExampleWindow();


protected:
    virtual void onInitialize() override;
    virtual void onResize(QResizeEvent * event) override;
    virtual void onPaint() override;


protected:
    void createAndSetupTexture();
    void createAndSetupGeometry();


protected:
    glo::ref_ptr<glo::Texture>                 m_texture;
    glo::ref_ptr<gloperate::ScreenAlignedQuad> m_quad;


};
