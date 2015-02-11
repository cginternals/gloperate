#include <gloperate-qtwidgets/ImageExporterWidget.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ImageExporter.h>

#include <gloperate-qt/QtOpenGLWindow.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_ImageExporterWidget.h"
#include <QAbstractButton>
#include <QFile>
#include <QFileDialog>
#include <QRegExp>
#include <QSettings>
#include <QString>
#include <QTime>
#include <QWindow>
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qtwidgets
{

ImageExporterWidget::ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:	QWidget(parent)
,	m_context(context)
,	m_ui(new Ui_ImageExporterWidget)
,	m_fileCounter(0)
{
	m_ui->setupUi(this);

	connect(m_ui->saveButton, &QPushButton::clicked,
		this, &ImageExporterWidget::handleSave);
	connect(m_ui->openDirectoryButton, &QPushButton::clicked, 
		this, &ImageExporterWidget::browseDirectory);
	connect(m_ui->fileNameLineEdit, &QLineEdit::textChanged,
		this, &ImageExporterWidget::checkFilename);
	connect(m_ui->fileNameLineEdit, &QLineEdit::editingFinished,
		this, &ImageExporterWidget::saveFilename);
	
	m_imageExporter = new gloperate::ImageExporter(painter, resourceManager);
	context->makeCurrent();
	m_imageExporter->initialize();
	context->doneCurrent();

	if (!gloperate::ImageExporter::isApplicableTo(painter))
		m_ui->saveButton->setDisabled(true);

	restoreSettings();
	updateDirectory();
	checkFilename(m_ui->fileNameLineEdit->text());
}

ImageExporterWidget::~ImageExporterWidget()
{
}

void ImageExporterWidget::updateDirectory()
{
	m_ui->directoryLineEdit->setText(m_dirName);
	QSettings settings;
	settings.beginGroup("ImageExporterWidget");
	settings.setValue("dirname", m_dirName);
	settings.endGroup();
}

void ImageExporterWidget::saveFilename()
{
	QSettings settings;
	settings.beginGroup("ImageExporterWidget");
	settings.setValue("filename", m_ui->fileNameLineEdit->text());
	settings.endGroup();
	m_fileCounter = 0;
}

void ImageExporterWidget::restoreSettings()
{
	QSettings settings;
	settings.beginGroup("ImageExporterWidget");
	m_dirName = settings.value("dirname", QDir::homePath()).toString();
	m_ui->fileNameLineEdit->setText(settings.value("filename", "image").toString());
	settings.endGroup();
}

void ImageExporterWidget::handleSave(bool checked)
{
	m_context->makeCurrent();
	m_imageExporter->save(buildFileName());
	m_context->doneCurrent();
}

void ImageExporterWidget::browseDirectory(bool checked)
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

std::string ImageExporterWidget::buildFileName()
{
	std::string filename{ (m_ui->fileNameLineEdit->text()).toStdString() };

	std::string num("<enum>");
	std::string tim("<timestamp>");
	std::string sep("/");
	std::string suf(".png");

	if (filename.find(num) != std::string::npos)
	{
		filename.replace(filename.find(num), num.length(), std::to_string(++m_fileCounter));
	}
	else if (filename.find(tim) != std::string::npos)
	{
		filename.replace(filename.find(tim), tim.length(), QDateTime::currentDateTime().toString("yyyy-dd-MM_hh-mm-ss-zzz").toStdString());
	}

	std::string final_filename = m_dirName.toStdString() + sep + filename + suf;
	int duplicate_count{ 2 };
	while (QFile::exists(QString::fromStdString(final_filename)))
		final_filename = m_dirName.toStdString() + sep + filename + " (" + std::to_string(duplicate_count++) + ")" + suf;

	return final_filename;
}

void ImageExporterWidget::checkFilename(const QString& text)
{
	QString num("<enum>");
	QString emp("");
	QString tim("<timestamp>");
	QString filename(text);

	if (filename.contains(num))
		filename.replace(filename.indexOf(num), num.length(), emp);
	if (filename.contains(tim))
		filename.replace(filename.indexOf(tim), tim.length(), emp);

	QRegExp rx("[A-Za-z0-9_\\-\\!\\§\\$\\%\\&\\(\\)\\=\\`\\´\\+\\'\\#\\-\\.\\,\\;\\_\\^\\°\\}\\{\\[\\]\\@\\x00C4\\x00E4\\x00D6\\x00F6\\x00DC\\x00FC\\x00DF\\s]{1,100}");
	
	if (!rx.exactMatch(filename))
	{
		if (m_ui->saveButton->isEnabled())
			m_ui->saveButton->setDisabled(true);
		
		m_ui->fileNameLineEdit->setStyleSheet("background-color:rgb(255,170,127);");
	}
	else
	{
		if (!m_ui->saveButton->isEnabled())
			m_ui->saveButton->setDisabled(false);

		m_ui->fileNameLineEdit->setStyleSheet(emp);
	}
}

} //namespace gloperate_qtwidgets
