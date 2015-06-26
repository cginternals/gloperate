
#include <gloperate-qt/widgets/ImageExporterResolutionWidget.h>

#include <cmath>

#include <gloperate/ext-includes-begin.h>
#include "ui_ImageExporterResolutionWidget.h"
#include <gloperate/ext-includes-end.h>


#define CM_PER_INCH 2.54
#define INCH_PER_CM 1 / CM_PER_INCH


namespace gloperate_qt
{


const QString ppiString = "ppi";
const QString pixelsPerCmString = "px/cm";
const QString pixelString = "pixel";
const QString inchString = "inch";
const QString cmString = "cm";


ImageExporterResolutionWidget::ImageExporterResolutionWidget(QWidget * parent)
: QWidget(parent)
, m_widthState(new ResolutionState(1920.0, pixelString))
, m_heightState(new ResolutionState(1080.0, pixelString))
, m_resolutionState(new ResolutionState(72, ppiString))
, m_ui(new Ui_ImageExporterResolutionWidget)
{
    m_ui->setupUi(this);

    void (QComboBox:: *comboBoxCurrentIndexChanged)(const QString &) = &QComboBox::currentIndexChanged;
    connect(m_ui->widthComboBox, comboBoxCurrentIndexChanged,
        this, &ImageExporterResolutionWidget::widthUnitChanged);
    connect(m_ui->heightComboBox, comboBoxCurrentIndexChanged,
        this, &ImageExporterResolutionWidget::heightUnitChanged);
    connect(m_ui->resolutionComboBox, comboBoxCurrentIndexChanged,
        this, &ImageExporterResolutionWidget::resolutionUnitChanged);

    void (QDoubleSpinBox:: *doubleSpinBoxValueChanged)(double d) = &QDoubleSpinBox::valueChanged;
    void (QSpinBox:: *spinBoxValueChanged)(int i) = &QSpinBox::valueChanged;
    connect(m_ui->widthDoubleSpinBox, doubleSpinBoxValueChanged,
        this, &ImageExporterResolutionWidget::widthValueChanged);
    connect(m_ui->heightDoubleSpinBox, doubleSpinBoxValueChanged,
        this, &ImageExporterResolutionWidget::heightValueChanged);
    connect(m_ui->resolutionSpinBox, spinBoxValueChanged,
        this, &ImageExporterResolutionWidget::resolutionValueChanged);

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
}

void ImageExporterResolutionWidget::updateResolutionSummary()
{
    // TODO: detect unsigned long long overflow
    int height{ static_cast<int>(std::round(toPixels(m_heightState->value, m_heightState->type))) };
    int width{ static_cast<int>(std::round(toPixels(m_widthState->value, m_widthState->type))) };
    emit resolutionChanged(QSize(width, height));

    unsigned long long pixelNumber{ static_cast<unsigned long long>(width) * static_cast<unsigned long long>(height) };
    QString unit;
    double byte;
    if (pixelNumber * 4 < 1024)
    {
        unit = "Byte";
        byte = static_cast<double>(pixelNumber) * 4;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 2)))
    {
        unit = "KiB";
        byte = static_cast<double>(pixelNumber) * 4 / 1024;
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 3)))
    {
        unit = "MiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 2);
    }
    else if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 4)))
    {
        unit = "GiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 3);
    }
    else //if (pixelNumber * 4 < static_cast<unsigned long long>(std::pow<int, int>(1024, 5)))
    {
        unit = "TiB";
        byte = static_cast<double>(pixelNumber) * 4 / std::pow<int, int>(1024, 4);
    }
    QString summary{ QString::number(pixelNumber) + " pixels (" + QString::number(std::round(byte * 100) / 100) + " " + unit + " uncompressed)" };
    //m_ui->resolutionSummaryLabel->setText(summary);
    emit resolutionSummaryChanged(summary);
}

double ImageExporterResolutionWidget::inchToPixels(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= m_resolutionState->value * CM_PER_INCH;

    return value;
}

double ImageExporterResolutionWidget::cmToPixels(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= m_resolutionState->value * INCH_PER_CM;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= m_resolutionState->value;

    return value;
}

double ImageExporterResolutionWidget::toPixels(double value, const QString& type)
{
    if (type == inchString)
        value = inchToPixels(value);
    else if (type == cmString)
        value = cmToPixels(value);

    return value;
}

double ImageExporterResolutionWidget::pixelsToCm(double value)
{
    if (m_resolutionState->type == ppiString)
        value *= CM_PER_INCH / m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value /= m_resolutionState->value;

    return value;
}

double ImageExporterResolutionWidget::pixelsToInch(double value)
{
    if (m_resolutionState->type == ppiString)
        value /= m_resolutionState->value;
    else if (m_resolutionState->type == pixelsPerCmString)
        value *= INCH_PER_CM / m_resolutionState->value;

    return value;
}

double ImageExporterResolutionWidget::pixelsTo(double value, const QString& type)
{
    if (type == inchString)
        value = pixelsToInch(value);
    else if (type == cmString)
        value = pixelsToCm(value);

    return value;
}

void ImageExporterResolutionWidget::setDecimals(QDoubleSpinBox* box, int dec)
{
    bool old = box->blockSignals(true);
    box->setDecimals(dec);
    box->blockSignals(old);
}

void ImageExporterResolutionWidget::widthUnitChanged(const QString& text)
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

void ImageExporterResolutionWidget::heightUnitChanged(const QString& text)
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

void ImageExporterResolutionWidget::widthValueChanged(double d)
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

void ImageExporterResolutionWidget::heightValueChanged(double d)
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

void ImageExporterResolutionWidget::resolutionValueChanged(int i)
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

void ImageExporterResolutionWidget::resolutionUnitChanged(const QString& text)
{
    if (text == ppiString)
        m_resolutionState->value = std::ceil(m_resolutionState->value * CM_PER_INCH);
    else
        m_resolutionState->value = std::ceil(m_resolutionState->value * INCH_PER_CM);

    m_ui->resolutionSpinBox->setValue((int)m_resolutionState->value);
    m_resolutionState->type = text;
}

void ImageExporterResolutionWidget::enableResolution(bool enable)
{
    m_ui->resolutionSpinBox->setEnabled(enable);
    m_ui->resolutionComboBox->setEnabled(enable);
    m_ui->resolutionLabel->setEnabled(enable);
}

ImageExporterResolutionWidget::~ImageExporterResolutionWidget()
{
}


} // namespace gloperate_qt
