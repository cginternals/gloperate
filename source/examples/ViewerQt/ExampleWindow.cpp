#include "ExampleWindow.h"
#include <gloperate-qt/qt-includes-begin.h>
#include <QResizeEvent>
#include <gloperate-qt/qt-includes-end.h>
#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>
#include <globjects/DebugMessage.h>


ExampleWindow::ExampleWindow()
{
}

ExampleWindow::~ExampleWindow()
{
}

void ExampleWindow::onInitialize()
{
    glo::init();

    glo::DebugMessage::enable();

    gl::glClearColor(0.2f, 0.3f, 0.4f, 1.f);

    createAndSetupTexture();
    createAndSetupGeometry();
}

void ExampleWindow::onResize(QResizeEvent * event)
{
    int width  = event->size().width();
    int height = event->size().height();
    gl::glViewport(0, 0, width, height);
}

void ExampleWindow::onPaint()
{
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);

    m_quad->draw();
}

void ExampleWindow::createAndSetupTexture()
{
    static const int w(256);
    static const int h(256);
    unsigned char data[w * h * 4];

    std::random_device rd;
    std::mt19937 generator(rd());
    std::poisson_distribution<> r(0.2);

    for (int i = 0; i < w * h * 4; ++i) {
        data[i] = static_cast<unsigned char>(255 - static_cast<unsigned char>(r(generator) * 255));
    }

    m_texture = glo::Texture::createDefault(gl::GL_TEXTURE_2D);
    m_texture->image2D(0, gl::GL_RGBA8, w, h, 0, gl::GL_RGBA, gl::GL_UNSIGNED_BYTE, data);
}

void ExampleWindow::createAndSetupGeometry()
{
    m_quad = new gloperate::ScreenAlignedQuad(m_texture);
    m_quad->setSamplerUniform(0);
}
