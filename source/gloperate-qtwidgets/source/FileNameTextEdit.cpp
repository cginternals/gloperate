#include <gloperate-qtwidgets/FileNameTextEdit.h>

#include <gloperate-qt/qt-includes-begin.h>
#include <gloperate-qtwidgets/FileNameTagCompleter.h>
#include <QKeyEvent>
#include <QListWidget>
#include <gloperate-qt/qt-includes-end.h>

#include <iostream>
#include <intrin.h>

namespace gloperate_qtwidgets
{

FileNameTextEdit::FileNameTextEdit(QWidget *parent)
:	QTextEdit(parent)
,	m_completer(nullptr)
,	m_textSelected(false)
,	m_fileNameListWidget(new QListWidget(parent))
{
	connect(this, &QTextEdit::copyAvailable,
		this, &FileNameTextEdit::textSelected);

	connect(this, &QTextEdit::cursorPositionChanged,
		this, &FileNameTextEdit::updateCompletionPrefix);

	m_fileNameListWidget->setParent(0, Qt::Popup);
	QPoint cursorPos = cursorRect().bottomLeft();
	m_fileNameListWidget->show();
	m_fileNameListWidget->setWindowFlags(Qt::CustomizeWindowHint);
	m_fileNameListWidget->setWindowFlags(Qt::FramelessWindowHint);
	m_fileNameListWidget->setGeometry(cursorPos.x() + geometry().x(), cursorPos.y() + geometry().y(), 100, 80);

	if (m_fileNameListWidget->count() == 0)
		m_fileNameListWidget->setVisible(false);
}

void FileNameTextEdit::initialize()
{
	connect(m_completer.get(), &FileNameTagCompleter::currentCompletionsChanged,
		this, &FileNameTextEdit::manageListWidget);
}

FileNameTextEdit::~FileNameTextEdit()
{
}

void FileNameTextEdit::setCompleter(std::shared_ptr<FileNameTagCompleter> completer)
{
	m_completer = completer;
}

std::shared_ptr<FileNameTagCompleter> FileNameTextEdit::completer()
{
	return m_completer;
}

void FileNameTextEdit::textSelected(bool b)
{
	m_textSelected = b;
}

void FileNameTextEdit::keyPressEvent(QKeyEvent * event)
{
	if (m_completer->hasCompletions() && m_fileNameListWidget->isVisible())
	{
		switch (event->key())
		{
		case Qt::Key_Return:
		case Qt::Key_Tab:
			completeTag();
			return;
		case Qt::Key_Escape:
			m_fileNameListWidget->hide();
			return;
		case Qt::Key_Up:
		case Qt::Key_Down:
			changeListIndex(event->key());
			return;
		default:
			QTextEdit::keyPressEvent(event);
		}
	}
	else if (event->key() != Qt::Key_Return)
		QTextEdit::keyPressEvent(event);
}

void FileNameTextEdit::completeTag()
{

}

void FileNameTextEdit::changeListIndex(int direction)
{
	if (direction == Qt::Key_Up)
		m_fileNameListWidget->setCurrentRow(std::max(m_fileNameListWidget->currentRow() - 1, 0));
	else
		m_fileNameListWidget->setCurrentRow(std::min(m_fileNameListWidget->currentRow() + 1, m_fileNameListWidget->count() - 1));
}

void FileNameTextEdit::closeListWidget()
{
	m_fileNameListWidget->close();
}

void FileNameTextEdit::updateCompletionPrefix()
{
	if (!m_textSelected)
	{
		int newPos = textCursor().position();
		int startIndex{ -1 };

		for (int searchIndex = newPos - 1; searchIndex >= 0; searchIndex--)
		{
			if ((newPos - 1) - searchIndex > m_completer->searchDepth)
				break;
			else if (toPlainText().at(searchIndex) == "<")
			{
				startIndex = searchIndex;
				break;
			}
		}
		QString prefix{ "" };
		QStringList completions{};
		if (startIndex >= 0)
		{
			for (startIndex; startIndex < newPos; startIndex++)
				prefix += toPlainText().at(startIndex);
		}
		else
			prefix += ">";

		m_completer->setCompletionPrefix(prefix);
		for (int i = 0; m_completer->setCurrentRow(i); i++)
			completions << m_completer->currentCompletion();
		m_completer->setCurrentCompletions(completions);
	}
	else if (m_completer->hasCompletions())
		m_completer->setCurrentCompletions({});
}

void FileNameTextEdit::manageListWidget()
{
	if (m_fileNameListWidget->count() > 0)
		m_fileNameListWidget->clear();

	if (m_completer->hasCompletions())
	{
		QRect cursorRect{ QTextEdit::cursorRect() };
		QPoint topLeft{ cursorRect.bottomLeft() };
		m_fileNameListWidget->move(topLeft.x() + geometry().x() + dynamic_cast<QWidget*>(parent())->geometry().x(), topLeft.y() + geometry().y() + dynamic_cast<QWidget*>(parent())->geometry().y());

		m_fileNameListWidget->addItems(m_completer->currentCompletions());
		m_fileNameListWidget->setCurrentRow(0);
	}
	
	if (!m_fileNameListWidget->isHidden() && m_fileNameListWidget->count() == 0)
	{
		m_fileNameListWidget->hide();
	}
	else if (m_fileNameListWidget->isHidden() && m_fileNameListWidget->count() > 0)
	{
		m_fileNameListWidget->show();
		m_fileNameListWidget->foregroundRole();
	}
}

} //namespace gloperate_qtwidgets
