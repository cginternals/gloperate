#include <gloperate-qtwidgets/ScreenshotWidget.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ScreenshotTool.h>

#include <gloperate-qt/QtOpenGLWindow.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_ScreenshotWidget.h"
#include <QAbstractButton>
#include <QFileDialog>
#include <QSettings>
#include <QString>
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
	connect(m_ui->openDirectoryButton, &QPushButton::clicked, 
		this, &ScreenshotWidget::browseDirectory);
	
	m_screenshotTool = new gloperate::ScreenshotTool(painter, resourceManager);
	context->makeCurrent();
	m_screenshotTool->initialize();
	context->doneCurrent();

	if (!gloperate::ScreenshotTool::isApplicableTo(painter))
		m_ui->buttonBox->buttons().first()->setDisabled(true);

	restoreSettings();
	updateDirectory();
}

ScreenshotWidget::~ScreenshotWidget()
{
}

void ScreenshotWidget::updateDirectory()
{
	m_ui->directoryLineEdit->setText(m_dirName);
	QSettings settings;
	settings.beginGroup("ScreenshotWidget");
	settings.setValue("dirname", m_dirName);
	settings.endGroup();
}

void ScreenshotWidget::restoreSettings()
{
	QSettings settings;
	settings.beginGroup("ScreenshotWidget");
	m_dirName = settings.value("dirname", QDir::homePath()).toString();
	settings.endGroup();
}

void ScreenshotWidget::handleSave(QAbstractButton* button)
{
	m_context->makeCurrent();
	m_screenshotTool->save(m_dirName.toStdString() + "/screenshot.png");
	m_context->doneCurrent();
}

void ScreenshotWidget::browseDirectory(bool checked)
{
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setOption(QFileDialog::ShowDirsOnly);
	dialog.setDirectory(m_dirName);

	if (dialog.exec() && !dialog.selectedFiles().empty())
	{
		m_dirName = dialog.selectedFiles().first();
		updateDirectory();
	}
}

} //namespace gloperate_qtwidgets
