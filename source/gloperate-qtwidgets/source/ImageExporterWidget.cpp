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

#include <algorithm>

#define CM_PER_INCH 2.54
#define INCH_PER_CM 1 / CM_PER_INCH

namespace gloperate_qtwidgets
{

const QString ppiString = "ppi";
const QString pixelsPerCmString = "px/cm";
const QString pixelString = "pixel";
const QString inchString = "inch";
const QString cmString = "cm";

ImageExporterWidget::ImageExporterWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:	QWidget(parent)
,	m_context(context)
,	m_ui(new Ui_ImageExporterWidget)
,	m_fileCounter(0)
,	m_widthState(new ResolutionState(1920.0, pixelString))
,	m_heightState(new ResolutionState(1080.0, pixelString))
,	m_resolutionState(new ResolutionState(72, ppiString))
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

	initializeResolutionGroupBox();

	restoreSettings();
	updateDirectory();
	checkFilename(m_ui->fileNameLineEdit->text());
}

void ImageExporterWidget::initializeResolutionGroupBox()
{
	void (QComboBox:: *comboBoxCurrentIndexChanged)(const QString &) = &QComboBox::currentIndexChanged;
	connect(m_ui->widthComboBox, comboBoxCurrentIndexChanged,
		this, &ImageExporterWidget::widthUnitChanged);
	connect(m_ui->heightComboBox, comboBoxCurrentIndexChanged,
		this, &ImageExporterWidget::heightUnitChanged);
	connect(m_ui->resolutionComboBox, comboBoxCurrentIndexChanged,
		this, &ImageExporterWidget::resolutionUnitChanged);

	void (QDoubleSpinBox:: *doubleSpinBoxValueChanged)(double d) = &QDoubleSpinBox::valueChanged;
	void (QSpinBox:: *spinBoxValueChanged)(int i) = &QSpinBox::valueChanged;
	connect(m_ui->widthDoubleSpinBox, doubleSpinBoxValueChanged,
		this, &ImageExporterWidget::widthValueChanged);
	connect(m_ui->heightDoubleSpinBox, doubleSpinBoxValueChanged,
		this, &ImageExporterWidget::heightValueChanged);
	connect(m_ui->resolutionSpinBox, spinBoxValueChanged,
		this, &ImageExporterWidget::resolutionValueChanged);

	m_ui->widthDoubleSpinBox->setRange(0.01, 100000.0);
	m_ui->heightDoubleSpinBox->setRange(0.01, 100000.0);

	QStringList units{ pixelString, inchString, cmString };
	m_ui->widthComboBox->addItems(units);
	m_ui->heightComboBox->addItems(units);

	QStringList resolutions{ ppiString, pixelsPerCmString };
	bool oldComboBoxSignalStatus = m_ui->resolutionComboBox->blockSignals(true);
	m_ui->resolutionComboBox->addItems(resolutions);
	m_ui->resolutionComboBox->blockSignals(oldComboBoxSignalStatus);

	bool oldSpinBoxSignalStatus = m_ui->resolutionSpinBox->blockSignals(true);
	m_ui->resolutionSpinBox->setValue(72);
	m_ui->resolutionSpinBox->blockSignals(oldSpinBoxSignalStatus);

	updateResolutionSummary();
}

void ImageExporterWidget::updateResolutionSummary()
{
	// TODO: detect unsigned long long overflow
	unsigned long long pixelNumber{ static_cast<unsigned long long>(std::round(toPixels(m_widthState->value, m_widthState->type)) * std::round(toPixels(m_heightState->value, m_heightState->type))) };
	QString unit;
	double byte;
	if (pixelNumber * 4 < 1024)
	{
		unit = "Byte";
		byte = pixelNumber * 4;
	}
	else if (pixelNumber * 4 < pow<unsigned long long>(1024, 2))
	{
		unit = "KiB";
		byte = pixelNumber * 4 / 1024;
	}
	else if (pixelNumber * 4 < pow<unsigned long long>(1024, 3))
	{
		unit = "MiB";
		byte = pixelNumber * 4 / pow<unsigned long long>(1024, 2);
	}
	else if (pixelNumber * 4 < pow<unsigned long long>(1024, 4))
	{
		unit = "GiB";
		byte = pixelNumber * 4 / pow<unsigned long long>(1024, 3);
	}
	else //if (pixelNumber * 4 < pow<unsigned long long>(1024, 5))
	{
		unit = "TiB";
		byte = pixelNumber * 4 / pow<unsigned long long>(1024, 4);
	}
	QString summary{ QString::number(pixelNumber) + " pixels (" + QString::number(std::round(byte * 100) / 100) + " " + unit + " uncompressed)" };
	m_ui->resolutionSummaryLabel->setText(summary);
}

double ImageExporterWidget::inchToPixels(double value)
{
	if (m_resolutionState->type == ppiString)
		value *= m_resolutionState->value;
	else if (m_resolutionState->type == pixelsPerCmString)
		value *= m_resolutionState->value * CM_PER_INCH;

	return value;
}

double ImageExporterWidget::cmToPixels(double value)
{
	if (m_resolutionState->type == ppiString)
		value *= m_resolutionState->value * INCH_PER_CM;
	else if (m_resolutionState->type == pixelsPerCmString)
		value *= m_resolutionState->value;

	return value;
}

double ImageExporterWidget::toPixels(double value, const QString& type)
{
	if (type == inchString)
		value = inchToPixels(value);
	else if (type == cmString)
		value = cmToPixels(value);

	return value;
}

double ImageExporterWidget::pixelsToCm(double value)
{
	if (m_resolutionState->type == ppiString)
		value *= CM_PER_INCH / m_resolutionState->value;
	else if (m_resolutionState->type == pixelsPerCmString)
		value /= m_resolutionState->value;

	return value;
}

double ImageExporterWidget::pixelsToInch(double value)
{
	if (m_resolutionState->type == ppiString)
		value /= m_resolutionState->value;
	else if (m_resolutionState->type == pixelsPerCmString)
		value *= INCH_PER_CM / m_resolutionState->value;

	return value;
}

double ImageExporterWidget::pixelsTo(double value, const QString& type)
{
	if (type == inchString)
		value = pixelsToInch(value);
	else if (type == cmString)
		value = pixelsToCm(value);

	return value;
}

void ImageExporterWidget::setDecimals(QDoubleSpinBox* box, int dec)
{
	bool old = box->blockSignals(true);
	box->setDecimals(dec);
	box->blockSignals(old);
}

void ImageExporterWidget::widthUnitChanged(const QString& text)
{
	if (text == pixelString)
	{
		m_widthState->value = toPixels(m_widthState->value, m_widthState->type);

		if (m_ui->heightComboBox->currentText() == pixelString)
			enableResolution(false);
		
		setDecimals(m_ui->widthDoubleSpinBox, 0);
	}
	else if (text == inchString)
	{
		if (m_widthState->type == pixelString)
		{
			m_widthState->value = pixelsToInch(m_widthState->value);

			setDecimals(m_ui->widthDoubleSpinBox, 2);
			enableResolution(true);
		}
		else if (m_widthState->type == cmString)
			m_widthState->value = m_widthState->value * INCH_PER_CM;
	}
	else
	{
		if (m_widthState->type == pixelString)
		{
			m_widthState->value = pixelsToCm(m_widthState->value);

			setDecimals(m_ui->widthDoubleSpinBox, 2);
			enableResolution(true);
		}
		else if (m_widthState->type == inchString)
			m_widthState->value = m_widthState->value * CM_PER_INCH;
	}

	m_ui->widthDoubleSpinBox->setValue(m_widthState->value);
	m_widthState->type = text;
}

void ImageExporterWidget::heightUnitChanged(const QString& text)
{
	if (text == pixelString)
	{
		m_heightState->value = toPixels(m_heightState->value, m_heightState->type);

		if (m_ui->widthComboBox->currentText() == pixelString)
			enableResolution(false);
		
		setDecimals(m_ui->heightDoubleSpinBox, 0);
	}
	else if (text == inchString)
	{
		if (m_heightState->type == pixelString)
		{
			m_heightState->value = pixelsToInch(m_heightState->value);

			setDecimals(m_ui->heightDoubleSpinBox, 2);
			enableResolution(true);
		}
		else if (m_heightState->type == cmString)
			m_heightState->value = m_heightState->value * INCH_PER_CM;
	}
	else
	{
		if (m_heightState->type == pixelString)
		{
			m_heightState->value = pixelsToCm(m_heightState->value);
			
			setDecimals(m_ui->heightDoubleSpinBox, 2);
			enableResolution(true);
		}
		else if (m_heightState->type == inchString)
			m_heightState->value = m_heightState->value * CM_PER_INCH;
	}

	m_ui->heightDoubleSpinBox->setValue(m_heightState->value);
	m_heightState->type = text;
}

void ImageExporterWidget::widthValueChanged(double d)
{
	if (m_ui->aspectCheckBox->isChecked())
	{
		double newValue{ toPixels(d, m_widthState->type) * toPixels(m_heightState->value, m_heightState->type) / toPixels(m_widthState->value, m_widthState->type) };
		m_heightState->value = pixelsTo(newValue, m_heightState->type);

		bool old = m_ui->heightDoubleSpinBox->blockSignals(true);
		if (m_heightState->type == pixelString)
			m_ui->heightDoubleSpinBox->setValue(std::round(m_heightState->value));
		else
			m_ui->heightDoubleSpinBox->setValue(m_heightState->value);
		m_ui->heightDoubleSpinBox->blockSignals(old);
	}
	
	m_widthState->value = d;

	updateResolutionSummary();
}

void ImageExporterWidget::heightValueChanged(double d)
{
	if (m_ui->aspectCheckBox->isChecked())
	{
		double newValue{ toPixels(d, m_heightState->type) * toPixels(m_widthState->value, m_widthState->type) / toPixels(m_heightState->value, m_heightState->type) };
		m_widthState->value = pixelsTo(newValue, m_widthState->type);

		bool old = m_ui->widthDoubleSpinBox->blockSignals(true);
		if (m_widthState->type == pixelString)
			m_ui->widthDoubleSpinBox->setValue(std::round(m_widthState->value));
		else
			m_ui->widthDoubleSpinBox->setValue(m_widthState->value);
		m_ui->widthDoubleSpinBox->blockSignals(old);
	}

	m_heightState->value = d;

	updateResolutionSummary();
}

void ImageExporterWidget::resolutionValueChanged(int i)
{
	double value;
	bool old;
	if (m_ui->aspectCheckBox->isChecked())
	{
		if (m_widthState->type == pixelString)
		{
			value = pixelsTo(m_widthState->value, m_heightState->type);
			m_resolutionState->value = i;
			m_widthState->value = toPixels(value, m_heightState->type);

			old = m_ui->widthDoubleSpinBox->blockSignals(true);
			m_ui->widthDoubleSpinBox->setValue(std::round(m_widthState->value));
			m_ui->widthDoubleSpinBox->blockSignals(old);
		}
		else if (m_heightState->type == pixelString)
		{
			value = pixelsTo(m_heightState->value, m_widthState->type);
			m_resolutionState->value = i;
			m_heightState->value = toPixels(value, m_widthState->type);

			old = m_ui->heightDoubleSpinBox->blockSignals(true);
			m_ui->heightDoubleSpinBox->setValue(std::round(m_heightState->value));
			m_ui->heightDoubleSpinBox->blockSignals(old);
		}
	}
	else
		m_resolutionState->value = i;

	updateResolutionSummary();
}

void ImageExporterWidget::resolutionUnitChanged(const QString& text)
{
	if (text == ppiString)
		m_resolutionState->value = std::ceil(m_resolutionState->value * CM_PER_INCH);
	else
		m_resolutionState->value = std::ceil(m_resolutionState->value * INCH_PER_CM);

	m_ui->resolutionSpinBox->setValue(m_resolutionState->value);
	m_resolutionState->type = text;
}

void ImageExporterWidget::enableResolution(bool enable)
{
	m_ui->resolutionSpinBox->setEnabled(enable);
	m_ui->resolutionComboBox->setEnabled(enable);
	m_ui->resolutionLabel->setEnabled(enable);
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
	m_imageExporter->save(buildFileName(), std::max(1, static_cast<int>(std::round(toPixels(m_widthState->value, m_widthState->type)))), std::max(1, static_cast<int>(std::round(toPixels(m_heightState->value, m_heightState->type)))), m_ui->renderIterationsSpinBox->value());
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
	std::string res("<resolution>");
	std::string sep("/");
	std::string suf(".png");

	if (filename.find(num) != std::string::npos)
	{
		// TODO: implement smart counter reset
		filename.replace(filename.find(num), num.length(), std::to_string(++m_fileCounter));
	}
	else if (filename.find(tim) != std::string::npos)
	{
		filename.replace(filename.find(tim), tim.length(), QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz").toStdString());
	}
	else if (filename.find(res) != std::string::npos)
	{
		filename.replace(filename.find(res), res.length(), std::to_string(static_cast<int>(std::round(toPixels(m_widthState->value, m_widthState->type)))) + "x" + std::to_string(static_cast<int>(std::round(toPixels(m_heightState->value, m_heightState->type)))));
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
	QString res("<resolution>");
	QString filename(text);

	if (filename.contains(num))
		filename.replace(filename.indexOf(num), num.length(), emp);
	if (filename.contains(tim))
		filename.replace(filename.indexOf(tim), tim.length(), emp);
	if (filename.contains(res))
		filename.replace(filename.indexOf(res), res.length(), emp);

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
