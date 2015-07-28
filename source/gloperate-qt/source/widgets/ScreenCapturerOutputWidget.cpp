
#include <gloperate-qt/widgets/ScreenCapturerOutputWidget.h>

#include <algorithm>
#include <set>

#include <gloperate/ext-includes-begin.h>

#include <QAbstractButton>
#include <QCompleter>
#include <QFileDialog>
#include <QSettings>
#include <QString>
#include <QStringListModel>
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


ScreenCapturerOutputWidget::ScreenCapturerOutputWidget(gloperate::ResourceManager & resourceManager, gloperate::Painter * painter, gloperate_qt::QtOpenGLWindow * context, gloperate::ScreenCapturer * screenCapturer, QWidget *parent)
:	QWidget(parent)
,	m_screenCapturer(screenCapturer)
,	m_context(context)
,	m_ui(new Ui_ScreenCapturerOutputWidget)
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

    if (!gloperate::ScreenCapturer::isApplicableTo(painter))
        m_ui->saveButton->setDisabled(true);

    m_screenCapturer->changeUiFilename.connect(this, &ScreenCapturerOutputWidget::updateUiFileNameEnumIndex);

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
        << "<second>";

    auto model = new QStringListModel(filenameTags);

    completer->setModel(model);
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);

    m_ui->fileNameTextEdit->setCompleter(completer);
}

void ScreenCapturerOutputWidget::updateResolutionSummaryLabel(const std::string& sizeSummary)
{
    m_ui->resolutionSummaryLabel->setText(QString::fromStdString(sizeSummary));
}

ScreenCapturerOutputWidget::~ScreenCapturerOutputWidget()
{
}

void ScreenCapturerOutputWidget::updateDirectory()
{
    m_ui->directoryLineEdit->setText(QString::fromStdString(m_screenCapturer->dirName()));
    QSettings settings;
    settings.beginGroup("ScreenCapturerOutputWidget");
    settings.setValue("dirname", QString::fromStdString(m_screenCapturer->dirName()));
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
    m_screenCapturer->setDirName(settings.value("dirname", QDir::homePath()).toString().toStdString());
    m_ui->fileNameTextEdit->setPlainText(settings.value("filename", "image").toString());
    settings.endGroup();
}

void ScreenCapturerOutputWidget::handleSave(bool)
{
    m_context->makeCurrent();
    m_screenCapturer->save(m_screenCapturer->buildFileName(m_ui->fileNameTextEdit->toPlainText().toStdString(), m_screenCapturer->width(), m_screenCapturer->height()), std::max(1, m_screenCapturer->width()), std::max(1, m_screenCapturer->height()));
    m_context->doneCurrent();
}

void ScreenCapturerOutputWidget::browseDirectory(bool)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setDirectory(QString::fromStdString(m_screenCapturer->dirName()));

    if (dialog.exec() && !dialog.selectedFiles().empty())
    {
        m_screenCapturer->setDirName(dialog.selectedFiles().first().toStdString());
        updateDirectory();
    }
}

void ScreenCapturerOutputWidget::updateUiFileNameEnumIndex()
{
    QString oldUiFilename{ m_ui->fileNameTextEdit->toPlainText() };
    int positionOfEnumNumIndex{ oldUiFilename.indexOf(QString::fromStdString(m_screenCapturer->supportedTags().at("enum_num"))) + QString::fromStdString(m_screenCapturer->supportedTags().at("enum_num")).length() };
    std::string startIndex{ m_screenCapturer->extractEnumNumStartIndex(oldUiFilename.toStdString(), positionOfEnumNumIndex) };
    QString newUiFilename{ oldUiFilename.replace(positionOfEnumNumIndex, static_cast<int>(startIndex.length()), QString::number(atoi(startIndex.c_str()) + 1)) };

    bool oldSignalStatus = m_ui->fileNameTextEdit->blockSignals(true);
    m_ui->fileNameTextEdit->setPlainText(newUiFilename);
    emit filenameChanged();
    saveFilename();
    m_ui->fileNameTextEdit->blockSignals(oldSignalStatus);
}

void ScreenCapturerOutputWidget::updateFilenamePreview()
{
    m_ui->filenamePeviewLabel->setText(QString::fromStdString(m_screenCapturer->replaceTags(m_ui->fileNameTextEdit->toPlainText().toStdString(), m_screenCapturer->width(), m_screenCapturer->height(), false)) + ".png");
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
