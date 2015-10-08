
#include <gloperate-qt/widgets/ImageExporterWidget.h>

#include <widgetzeug/DataLinkWidget.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ImageExporter.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/ImageExporterMultiFrameWidget.h>
#include <gloperate-qt/widgets/ImageExporterOutputWidget.h>
#include <gloperate-qt/widgets/ImageExporterResolutionWidget.h>
#include <gloperate-qt/widgets/ImageExporterTilebasedWidget.h>


namespace gloperate_qt
{


ImageExporterWidget::ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
: DockableScrollAreaWidget(parent)
, m_outputWidget(new ImageExporterOutputWidget(resourceManager, painter, context, this))
, m_resolutionWidget(new ImageExporterResolutionWidget(this))
, m_multiFrameWidget(new ImageExporterMultiFrameWidget(this))
, m_tilebasedWidget(new ImageExporterTilebasedWidget(this))
, m_dataLinkWidget(new widgetzeug::DataLinkWidget(this))
{
    connect(m_resolutionWidget.get(), &ImageExporterResolutionWidget::resolutionSummaryChanged,
        m_outputWidget.get(), &ImageExporterOutputWidget::updateResolutionSummaryLabel);

    connect(m_resolutionWidget.get(), &ImageExporterResolutionWidget::resolutionChanged,
        m_outputWidget.get(), &ImageExporterOutputWidget::updateResolution);

    connect(m_multiFrameWidget.get(), &ImageExporterMultiFrameWidget::frameCountChanged,
        m_outputWidget.get(), &ImageExporterOutputWidget::updateFrameCount);

    addWidget(m_dataLinkWidget.get());
    addWidget(m_outputWidget.get());
    addWidget(m_resolutionWidget.get());
    addWidget(m_multiFrameWidget.get());
    addWidget(m_tilebasedWidget.get());


    m_resolutionWidget->updateResolutionSummary();
}

ImageExporterWidget::~ImageExporterWidget()
{
}


} // namespace gloperate_qt
