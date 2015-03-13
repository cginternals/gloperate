#include <gloperate-qtwidgets/FileNameInputWidget.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_FileNameInputWidget.h"
#include <gloperate-qtwidgets/FileNameTagCompleter.h>
#include <QKeyEvent>
#include <QTextEdit>
#include <QCompleter>
#include <QString>
#include <QTextCursor>
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qtwidgets
{

FileNameInputWidget::FileNameInputWidget(FileNameTagCompleter * completer, QString & initialFilename, QWidget *parent)
:	QWidget(parent)
,	m_ui(new Ui_FileNameInputWidget)
,	m_completer(completer)
{
	m_ui->setupUi(this);
	m_ui->fileNameTextEdit->setCompleter(m_completer);
	m_ui->fileNameTextEdit->initialize();

	m_ui->fileNameTextEdit->setText(initialFilename);
}

FileNameInputWidget::~FileNameInputWidget()
{
}

void FileNameInputWidget::closeEvent(QCloseEvent * event)
{
	m_ui->fileNameTextEdit->closeListWidget();
	QWidget::closeEvent(event);
}

} //namespace gloperate_qtwidgets
