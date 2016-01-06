
#include <gloperate-qt/widgets/ImageExporterOutputWidget.h>

#include <algorithm>
#include <set>

#include <gloperate/ext-includes-begin.h>

#include <QAbstractButton>
#include <QCompleter>
#include <QFile>
#include <QFileDialog>
#include <QRegExp>
#include <QSettings>
#include <QString>
#include <QStringListModel>
#include <QTime>
#include <QWindow>

#include "ui_ImageExporterOutputWidget.h"

#include <gloperate/ext-includes-end.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ImageExporter.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/FileNameTextEdit.h>


namespace gloperate_qt
{


ImageExporterOutputWidget::ImageExporterOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:	QWidget(parent)
,	m_context(context)
,	m_supportedTags({ { "width", "<width>" }, { "height", "<height>" }, { "enum_num", "<enum#" }, { "year", "<year>" }, { "month", "<month>" }, { "day", "<day>" }, { "hour", "<hour>" }, { "minute", "<minute>" }, { "second", "<second>" }, { "millisec", "<millisecond>" } })
,	m_ui(new Ui_ImageExporterOutputWidget)
,	m_resolution(new QSize(1,1))
,   m_count(1)
{
    m_ui->setupUi(this);

    m_ui->fileNameTextEdit->setMaximumHeight(m_ui->directoryLineEdit->height() * 2);
    initializeFileNameTextEdit();

    connect(m_ui->saveButton, &QPushButton::clicked,
        this, &ImageExporterOutputWidget::handleSave);
    connect(m_ui->openDirectoryButton, &QPushButton::clicked, 
        this, &ImageExporterOutputWidget::browseDirectory);
    connect(m_ui->fileNameTextEdit, &FileNameTextEdit::textChanged,
        this, &ImageExporterOutputWidget::checkFilename);
    connect(m_ui->fileNameTextEdit, &FileNameTextEdit::textChanged,
        this, &ImageExporterOutputWidget::saveFilename);

    connect(this, &ImageExporterOutputWidget::filenameChanged,
        this, &ImageExporterOutputWidget::updateFilenamePreview);
    
    m_imageExporter = new gloperate::ImageExporter(painter, resourceManager);
    context->makeCurrent();
    m_imageExporter->initialize();
    context->doneCurrent();

    if (!gloperate::ImageExporter::isApplicableTo(painter))
        m_ui->saveButton->setDisabled(true);

    restoreSettings();
    updateDirectory();
    checkFilename();
}

void ImageExporterOutputWidget::initializeFileNameTextEdit()
{
    auto completer = new QCompleter;

    const QStringList filenameTags = QStringList()
        << "<width>"
        << "<height>"
        << "<enum#"
        << "<year>"
        << "<month>"
        << "<day>"
        << "<hour>"
        << "<minute>"
        << "<second>"
        << "<millisecond>";

    auto model = new QStringListModel(filenameTags);

    completer->setModel(model);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);

    m_ui->fileNameTextEdit->setCompleter(completer);
}

void ImageExporterOutputWidget::updateResolutionSummaryLabel(const QString& sizeSummary)
{
    m_ui->resolutionSummaryLabel->setText(sizeSummary);
}

void ImageExporterOutputWidget::updateResolution(const QSize& resolution)
{
    m_resolution->setHeight(resolution.height());
    m_resolution->setWidth(resolution.width());
}

ImageExporterOutputWidget::~ImageExporterOutputWidget()
{
}

void ImageExporterOutputWidget::updateDirectory()
{
    m_ui->directoryLineEdit->setText(m_dirName);
    QSettings settings;
    settings.beginGroup("ImageExporterOutputWidget");
    settings.setValue("dirname", m_dirName);
    settings.endGroup();
}

void ImageExporterOutputWidget::saveFilename()
{
    QSettings settings;
    settings.beginGroup("ImageExporterOutputWidget");
    settings.setValue("filename", m_ui->fileNameTextEdit->toPlainText());
    settings.endGroup();
}

void ImageExporterOutputWidget::restoreSettings()
{
    QSettings settings;
    settings.beginGroup("ImageExporterOutputWidget");
    m_dirName = settings.value("dirname", QDir::homePath()).toString();
    m_ui->fileNameTextEdit->setPlainText(settings.value("filename", "image").toString());
    settings.endGroup();
}

void ImageExporterOutputWidget::handleSave(bool)
{
    m_context->makeCurrent();
    m_imageExporter->save(buildFileName(), std::max(1, m_resolution->width()), std::max(1, m_resolution->height()), std::max(1, m_count));
    m_context->doneCurrent();
}

void ImageExporterOutputWidget::browseDirectory(bool)
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

std::string ImageExporterOutputWidget::replaceTags(const std::string& filename, bool shouldUpdateUiFilename)
{
    QDateTime time{ QDateTime::currentDateTime() };
    std::string newFilename{ filename };

    if (newFilename.find(m_supportedTags["width"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["width"].toStdString()), m_supportedTags["width"].toStdString().length(), std::to_string(m_resolution->width()));

    if (newFilename.find(m_supportedTags["height"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["height"].toStdString()), m_supportedTags["height"].toStdString().length(), std::to_string(m_resolution->height()));

    if (newFilename.find(m_supportedTags["day"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["day"].toStdString()), m_supportedTags["day"].toStdString().length(), time.toString("dd").toStdString());

    if (newFilename.find(m_supportedTags["month"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["month"].toStdString()), m_supportedTags["month"].toStdString().length(), time.toString("MM").toStdString());

    if (newFilename.find(m_supportedTags["year"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["year"].toStdString()), m_supportedTags["year"].toStdString().length(), time.toString("yyyy").toStdString());

    if (newFilename.find(m_supportedTags["hour"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["hour"].toStdString()), m_supportedTags["hour"].toStdString().length(), time.toString("hh").toStdString());

    if (newFilename.find(m_supportedTags["minute"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["minute"].toStdString()), m_supportedTags["minute"].toStdString().length(), time.toString("mm").toStdString());

    if (newFilename.find(m_supportedTags["second"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["second"].toStdString()), m_supportedTags["second"].toStdString().length(), time.toString("ss").toStdString());

    if (newFilename.find(m_supportedTags["millisec"].toStdString()) != std::string::npos)
        newFilename.replace(newFilename.find(m_supportedTags["millisec"].toStdString()), m_supportedTags["millisec"].toStdString().length(), time.toString("zzz").toStdString());
    
    if (newFilename.find(m_supportedTags["enum_num"].toStdString()) != std::string::npos)
    {
        auto position = newFilename.find(m_supportedTags["enum_num"].toStdString());
        newFilename.replace(position, m_supportedTags["enum_num"].length(), "");

        auto startIndex = extractEnumNumStartIndex(newFilename, position);

        int index = atoi(startIndex.c_str());
        newFilename.replace(position, startIndex.length() + 1, std::to_string(index));

        if (shouldUpdateUiFilename)
            updateUiFileName();
    }

    return newFilename;
}

std::string ImageExporterOutputWidget::extractEnumNumStartIndex(const std::string& filename, size_t position)
{
    auto startIndex = std::string("");

    while (filename.at(position) != '>')
    {
        startIndex += filename.at(position);
        position++;
    }

    return startIndex;
}

std::string ImageExporterOutputWidget::buildFileName()
{
    std::string filename{ replaceTags(m_ui->fileNameTextEdit->toPlainText().toStdString()) };
    
    const std::string sep("/");
    const std::string suf(".png");

    std::string final_filename = m_dirName.toStdString() + sep + filename + suf;
    int duplicate_count{ 2 };
    while (QFile::exists(QString::fromStdString(final_filename)))
        final_filename = m_dirName.toStdString() + sep + filename + " (" + std::to_string(duplicate_count++) + ")" + suf;

    return final_filename;
}

void ImageExporterOutputWidget::updateUiFileName()
{
    QString oldUiFilename{ m_ui->fileNameTextEdit->toPlainText() };
    size_t positionOfEnumNumIndex = oldUiFilename.indexOf(m_supportedTags["enum_num"]) + m_supportedTags["enum_num"].length();
    std::string startIndex{ extractEnumNumStartIndex(oldUiFilename.toStdString(), positionOfEnumNumIndex) };
    QString newUiFilename{ oldUiFilename.replace(positionOfEnumNumIndex, static_cast<int>(startIndex.length()), QString::number(atoi(startIndex.c_str()) + 1)) };

    bool oldSignalStatus = m_ui->fileNameTextEdit->blockSignals(true);
    m_ui->fileNameTextEdit->setPlainText(newUiFilename);
    emit filenameChanged();
    saveFilename();
    m_ui->fileNameTextEdit->blockSignals(oldSignalStatus);
}

void ImageExporterOutputWidget::updateFilenamePreview()
{
    m_ui->filenamePeviewLabel->setText(QString::fromStdString(replaceTags(m_ui->fileNameTextEdit->toPlainText().toStdString(), false)) + ".png");
}

void ImageExporterOutputWidget::checkFilename()
{
    const QString emp("");
    QString filename(m_ui->fileNameTextEdit->toPlainText());
    QString errorMessage{ "" };

    for (auto it = m_supportedTags.begin(); it != m_supportedTags.end(); it++)
    {
        if (filename.contains(it->second) && it->first != "enum_num")
            filename.replace(filename.indexOf(it->second), it->second.length(), emp);
    }
    
    if (filename.indexOf(m_supportedTags["enum_num"]) != static_cast<int>(std::string::npos))
    {
        int position = filename.indexOf(m_supportedTags["enum_num"]);
        filename.replace(position, static_cast<int>(m_supportedTags["enum_num"].length()), "");

        std::set<QString> numbers{ "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
        bool numberFound{ false }, endFound{ false };

        while (position < filename.length())
        {
            if (numbers.find(filename.at(position)) != numbers.end())
            {
                filename.replace(position, 1, "");
                numberFound = true;
            }
            else if (numberFound && filename.at(position) == '>')
            {
                filename.replace(position, 1, "");
                endFound = true;
                break;
            }
            else
                break;
        }

        if (!endFound)
            errorMessage = "includes an incomplete <enum#> tag";
    }
    
    if (errorMessage == "")
    {
        QRegExp rx("[A-Za-z0-9_\\-\\!\\§\\$\\%\\&\\(\\)\\=\\`\\´\\+\\'\\#\\-\\.\\,\\;\\_\\^\\°\\}\\{\\[\\]\\@\\x00C4\\x00E4\\x00D6\\x00F6\\x00DC\\x00FC\\x00DF\\s]{1,100}");

        if (!rx.exactMatch(filename))
            errorMessage = "includes invalid symbols";
    }

    if (errorMessage != "")
    {
        if (m_ui->saveButton->isEnabled())
            m_ui->saveButton->setDisabled(true);
        
        m_ui->fileNameTextEdit->setStyleSheet("background-color:rgb(255,170,127);");
        m_ui->filenamePeviewLabel->setText( filename + ".png " + errorMessage);
    }
    else
    {
        if (!m_ui->saveButton->isEnabled())
            m_ui->saveButton->setDisabled(false);

        m_ui->fileNameTextEdit->setStyleSheet(emp);

        emit filenameChanged();
    }
}

void ImageExporterOutputWidget::updateFrameCount(int count)
{
    m_count = count;
}


} // namespace gloperate_qt
