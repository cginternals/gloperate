
#include <gloperate-qt/widgets/ScreenCapturerOutputWidget.h>

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

#include "ui_ScreenCapturerOutputWidget.h"

#include <gloperate/ext-includes-end.h>

#include <gloperate/resources/ResourceManager.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/tools/ScreenCapturer.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>
#include <gloperate-qt/widgets/FileNameTextEdit.h>


namespace gloperate_qt
{


ScreenCapturerOutputWidget::ScreenCapturerOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, QWidget *parent)
:	QWidget(parent)
,	m_context(context)
,	m_ui(new Ui_ScreenCapturerOutputWidget)
,	m_resolution(new QSize(1,1))
{
    m_ui->setupUi(this);

    m_ui->fileNameTextEdit->setMaximumHeight(m_ui->directoryLineEdit->height() * 2);
    initializeFileNameTextEdit();

    connect(m_ui->saveButton, &QPushButton::clicked,
        this, &ScreenCapturerOutputWidget::handleSave);
    connect(m_ui->openDirectoryButton, &QPushButton::clicked, 
        this, &ScreenCapturerOutputWidget::browseDirectory);
    connect(m_ui->fileNameTextEdit, &FileNameTextEdit::textChanged,
        this, &ScreenCapturerOutputWidget::checkFilename);
    connect(m_ui->fileNameTextEdit, &FileNameTextEdit::textChanged,
        this, &ScreenCapturerOutputWidget::saveFilename);

    connect(this, &ScreenCapturerOutputWidget::filenameChanged,
        this, &ScreenCapturerOutputWidget::updateFilenamePreview);
    
    m_screenCapturer = new gloperate::ScreenCapturer(painter, resourceManager);
    context->makeCurrent();
    m_screenCapturer->initialize();
    context->doneCurrent();

    if (!gloperate::ScreenCapturer::isApplicableTo(painter))
        m_ui->saveButton->setDisabled(true);

    restoreSettings();
    updateDirectory();
    checkFilename();
}

void ScreenCapturerOutputWidget::initializeFileNameTextEdit()
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

void ScreenCapturerOutputWidget::updateResolutionSummaryLabel(const QString& sizeSummary)
{
    m_ui->resolutionSummaryLabel->setText(sizeSummary);
}

void ScreenCapturerOutputWidget::updateResolution(const QSize& resolution)
{
    m_resolution->setHeight(resolution.height());
    m_resolution->setWidth(resolution.width());
}

ScreenCapturerOutputWidget::~ScreenCapturerOutputWidget()
{
}

void ScreenCapturerOutputWidget::updateDirectory()
{
    m_ui->directoryLineEdit->setText(m_dirName);
    QSettings settings;
    settings.beginGroup("ScreenCapturerOutputWidget");
    settings.setValue("dirname", m_dirName);
    settings.endGroup();
}

void ScreenCapturerOutputWidget::saveFilename()
{
    QSettings settings;
    settings.beginGroup("ScreenCapturerOutputWidget");
    settings.setValue("filename", m_ui->fileNameTextEdit->toPlainText());
    settings.endGroup();
}

void ScreenCapturerOutputWidget::restoreSettings()
{
    QSettings settings;
    settings.beginGroup("ScreenCapturerOutputWidget");
    m_dirName = settings.value("dirname", QDir::homePath()).toString();
    m_ui->fileNameTextEdit->setPlainText(settings.value("filename", "image").toString());
    settings.endGroup();
}

void ScreenCapturerOutputWidget::handleSave(bool)
{
    m_context->makeCurrent();
    m_screenCapturer->save(buildFileName(), std::max(1, m_resolution->width()), std::max(1, m_resolution->height()));
    m_context->doneCurrent();
}

void ScreenCapturerOutputWidget::browseDirectory(bool)
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

std::string ScreenCapturerOutputWidget::replaceTags(const std::string& filename, bool shouldUpdateUiFilename)
{
    QDateTime time{ QDateTime::currentDateTime() };
    std::string newFilename{ filename };

    if (newFilename.find(m_screenCapturer->supportedTags().at("width")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("width")), m_screenCapturer->supportedTags().at("width").length(), std::to_string(m_resolution->width()));

    if (newFilename.find(m_screenCapturer->supportedTags().at("height")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("height")), m_screenCapturer->supportedTags().at("height").length(), std::to_string(m_resolution->height()));

    if (newFilename.find(m_screenCapturer->supportedTags().at("day")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("day")), m_screenCapturer->supportedTags().at("day").length(), time.toString("dd").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("month")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("month")), m_screenCapturer->supportedTags().at("month").length(), time.toString("MM").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("year")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("year")), m_screenCapturer->supportedTags().at("year").length(), time.toString("yyyy").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("hour")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("hour")), m_screenCapturer->supportedTags().at("hour").length(), time.toString("hh").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("minute")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("minute")), m_screenCapturer->supportedTags().at("minute").length(), time.toString("mm").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("second")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("second")), m_screenCapturer->supportedTags().at("second").length(), time.toString("ss").toStdString());

    if (newFilename.find(m_screenCapturer->supportedTags().at("millisec")) != std::string::npos)
        newFilename.replace(newFilename.find(m_screenCapturer->supportedTags().at("millisec")), m_screenCapturer->supportedTags().at("millisec").length(), time.toString("zzz").toStdString());
    
    if (newFilename.find(m_screenCapturer->supportedTags().at("enum_num")) != std::string::npos)
    {
        size_t position = newFilename.find(m_screenCapturer->supportedTags().at("enum_num"));
        newFilename.replace(position, m_screenCapturer->supportedTags().at("enum_num").length(), "");

        std::string startIndex{ extractEnumNumStartIndex(newFilename, static_cast<int>(position)) };

        int index{ atoi(startIndex.c_str()) };
        newFilename.replace(position, startIndex.length() + 1, std::to_string(index));

        if (shouldUpdateUiFilename)
            updateUiFileName();
    }

    return newFilename;
}

std::string ScreenCapturerOutputWidget::extractEnumNumStartIndex(const std::string& filename, int position)
{
    std::string startIndex{ "" };

    while (filename.at(position) != '>')
    {
        startIndex += filename.at(position);
        position++;
    }

    return startIndex;
}

std::string ScreenCapturerOutputWidget::buildFileName()
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

void ScreenCapturerOutputWidget::updateUiFileName()
{
    QString oldUiFilename{ m_ui->fileNameTextEdit->toPlainText() };
    int positionOfEnumNumIndex{ oldUiFilename.indexOf(QString::fromStdString(m_screenCapturer->supportedTags().at("enum_num"))) + QString::fromStdString(m_screenCapturer->supportedTags().at("enum_num")).length() };
    std::string startIndex{ extractEnumNumStartIndex(oldUiFilename.toStdString(), positionOfEnumNumIndex) };
    QString newUiFilename{ oldUiFilename.replace(positionOfEnumNumIndex, static_cast<int>(startIndex.length()), QString::number(atoi(startIndex.c_str()) + 1)) };

    bool oldSignalStatus = m_ui->fileNameTextEdit->blockSignals(true);
    m_ui->fileNameTextEdit->setPlainText(newUiFilename);
    emit filenameChanged();
    saveFilename();
    m_ui->fileNameTextEdit->blockSignals(oldSignalStatus);
}

void ScreenCapturerOutputWidget::updateFilenamePreview()
{
    m_ui->filenamePeviewLabel->setText(QString::fromStdString(replaceTags(m_ui->fileNameTextEdit->toPlainText().toStdString(), false)) + ".png");
}

void ScreenCapturerOutputWidget::checkFilename()
{
    QString fileName{ m_ui->fileNameTextEdit->toPlainText() };
    QString errorMessage{ QString::fromStdString(m_screenCapturer->checkFilename(fileName.toStdString())) };
    if (errorMessage != "")
    {
        if (m_ui->saveButton->isEnabled())
            m_ui->saveButton->setDisabled(true);

        m_ui->fileNameTextEdit->setStyleSheet("background-color:rgb(255,170,127);");
        m_ui->filenamePeviewLabel->setText(fileName + ".png " + errorMessage);
    }
    else
    {
        if (!m_ui->saveButton->isEnabled())
            m_ui->saveButton->setDisabled(false);

        m_ui->fileNameTextEdit->setStyleSheet("");

        emit filenameChanged();
    }
}
} // namespace gloperate_qt
