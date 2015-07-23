
#include <gloperate-qt/widgets/ScreenCapturerWidget.h>

#include <widgetzeug/DataLinkWidget.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/ScreenCapturerOutputWidget.h>
#include <gloperate-qt/widgets/ScreenCapturerResolutionWidget.h>
#include <gloperate-qt/widgets/ScreenCapturerTilebasedWidget.h>


namespace gloperate_qt
{


ScreenCapturerWidget::ScreenCapturerWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:   DockableScrollAreaWidget(parent)
,   m_outputWidget(new ScreenCapturerOutputWidget(resourceManager, painter, context, this))
,   m_resolutionWidget(new ScreenCapturerResolutionWidget(this))
,   m_tilebasedWidget(new ScreenCapturerTilebasedWidget(this))
,   m_dataLinkWidget(new widgetzeug::DataLinkWidget(this))
{
    connect(m_resolutionWidget.get(), &ScreenCapturerResolutionWidget::resolutionSummaryChanged,
        m_outputWidget.get(), &ScreenCapturerOutputWidget::updateResolutionSummaryLabel);

    connect(m_resolutionWidget.get(), &ScreenCapturerResolutionWidget::resolutionChanged,
        m_outputWidget.get(), &ScreenCapturerOutputWidget::updateResolution);

    addWidget(m_dataLinkWidget.get());
    addWidget(m_outputWidget.get());
    addWidget(m_resolutionWidget.get());
    addWidget(m_tilebasedWidget.get());


    m_resolutionWidget->updateResolutionSummary();
}

ScreenCapturerWidget::~ScreenCapturerWidget()
{
}


} // namespace gloperate_qt
