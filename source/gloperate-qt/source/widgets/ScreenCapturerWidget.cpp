
#include <gloperate-qt/widgets/ScreenCapturerWidget.h>

#include <widgetzeug/DataLinkWidget.h>

#include <signalzeug/Signal.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ScreenCapturer.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/ScreenCapturerOutputWidget.h>
#include <gloperate-qt/widgets/ScreenCapturerResolutionWidget.h>
#include <gloperate-qt/widgets/ScreenCapturerTilebasedWidget.h>


namespace gloperate_qt
{


ScreenCapturerWidget::ScreenCapturerWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:   DockableScrollAreaWidget(parent)
,   m_tilebasedWidget(new ScreenCapturerTilebasedWidget(this))
,   m_dataLinkWidget(new widgetzeug::DataLinkWidget(this))
,   m_screenCapturer(new gloperate::ScreenCapturer(painter, resourceManager))
{
    context->makeCurrent();
    m_screenCapturer->initialize();
    context->doneCurrent();

    m_outputWidget.reset(new ScreenCapturerOutputWidget(resourceManager, painter, context, m_screenCapturer.get(), this));
    m_resolutionWidget.reset(new ScreenCapturerResolutionWidget(m_screenCapturer.get(), this));

    m_screenCapturer->resolutionSummaryChanged.connect(m_outputWidget.get(), &ScreenCapturerOutputWidget::updateResolutionSummaryLabel);

    addWidget(m_dataLinkWidget.get());
    addWidget(m_outputWidget.get());
    addWidget(m_resolutionWidget.get());
    addWidget(m_tilebasedWidget.get());


    m_screenCapturer->createResolutionSummary();
}

ScreenCapturerWidget::~ScreenCapturerWidget()
{
}


} // namespace gloperate_qt
