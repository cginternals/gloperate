#include <gloperate-qtwidgets/ScreenshotWidget.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ScreenshotTool.h>

#include <gloperate-qt/QtOpenGLWindow.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_ScreenshotWidget.h"
#include <QAbstractButton>
#include <QWindow>
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qtwidgets
{

ScreenshotWidget::ScreenshotWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:	QWidget(parent)
,	m_context(context)
,	m_ui(new Ui_ScreenshotWidget)
{
	m_ui->setupUi(this);

	connect(m_ui->buttonBox, &QDialogButtonBox::clicked,
		this, &ScreenshotWidget::handleSave);
	
	m_screenshotTool = new gloperate::ScreenshotTool(painter, resourceManager);
	context->makeCurrent();
	m_screenshotTool->initialize();
	context->doneCurrent();

	if (!gloperate::ScreenshotTool::isApplicableTo(painter))
		m_ui->buttonBox->buttons().first()->setDisabled(true);
}

ScreenshotWidget::~ScreenshotWidget()
{
}

void ScreenshotWidget::handleSave(QAbstractButton* button)
{
	m_context->makeCurrent();
	m_screenshotTool->save("screenshot.png");
	m_context->doneCurrent();
}

} //namespace gloperate_qtwidgets
