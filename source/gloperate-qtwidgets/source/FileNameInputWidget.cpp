#include <gloperate-qtwidgets/FileNameInputWidget.h>

#include <gloperate-qt/qt-includes-begin.h>
#include "ui_FileNameInputWidget.h"
#include <QKeyEvent>
#include <QTextEdit>
#include <QCompleter>
#include <QString>
#include <gloperate-qt/qt-includes-end.h>

namespace gloperate_qtwidgets
{

const int searchDepth = 13;

FileNameInputWidget::FileNameInputWidget(QCompleter * completer, QWidget *parent)
:	QWidget(parent)
,	m_ui(new Ui_FileNameInputWidget)
,	m_completer(completer)
{
	m_ui->setupUi(this);
	m_ui->fileNameLineEdit->setCompleter(m_completer);
	m_completer->popup()->setMaximumSize(0, 0);

	connect(m_ui->fileNameLineEdit, &QLineEdit::cursorPositionChanged,
		this, &FileNameInputWidget::correctCompleterPrefix);

	connect(this, &FileNameInputWidget::setCompletionPrefix,
		m_ui->fileNameLineEdit->completer(), &QCompleter::setCompletionPrefix);

	if (m_ui->listWidget->count() == 0)
		m_ui->listWidget->setVisible(false);
}

FileNameInputWidget::~FileNameInputWidget()
{
}

void FileNameInputWidget::correctCompleterPrefix(int oldPos, int newPos)
{
	if (m_ui->listWidget->count() > 0)
		m_ui->listWidget->clear();

	if (!m_ui->fileNameLineEdit->hasSelectedText())
	{
		int startIndex{ -1 };

		for (int searchIndex = newPos - 1; searchIndex >= 0; searchIndex--)
		{
			if ((newPos - 1) - searchIndex > searchDepth)
				break;
			else if (m_ui->fileNameLineEdit->text().at(searchIndex) == "<")
			{
				startIndex = searchIndex;
				break;
			}
		}
		QString prefix{ "" };
		if (startIndex >= 0)
		{
			for (startIndex; startIndex < newPos; startIndex++)
				prefix += m_ui->fileNameLineEdit->text().at(startIndex);

			setCompletionPrefix(prefix);
			for (int i = 0; m_completer->setCurrentRow(i); i++)
				m_ui->listWidget->addItem(m_completer->currentCompletion());
		}
	}

	if (m_ui->listWidget->isVisible() && m_ui->listWidget->count() == 0)
		m_ui->listWidget->setVisible(false);
	else if (!m_ui->listWidget->isVisible() && m_ui->listWidget->count() > 0)
		m_ui->listWidget->setVisible(true);
}

} //namespace gloperate_qtwidgets
