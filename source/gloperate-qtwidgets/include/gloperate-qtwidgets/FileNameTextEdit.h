#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QTextEdit>
#include <memory>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class QKeyEvent;
class QString;
class QListWidget;

namespace gloperate_qtwidgets
{

class FileNameTagCompleter;

class GLOPERATE_QTWIDGETS_API FileNameTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	FileNameTextEdit(QWidget * parent = nullptr);
	virtual ~FileNameTextEdit();

	virtual void setCompleter(std::shared_ptr<FileNameTagCompleter> completer);
	virtual std::shared_ptr<FileNameTagCompleter> completer();
	virtual void updateCompletionPrefix();
	void closeListWidget();
	virtual void initialize();

protected:
	void textSelected(bool b);
	virtual void keyPressEvent(QKeyEvent * event);
	void manageListWidget();
	void completeTag();
	void changeListIndex(int direction);

protected:
	std::shared_ptr<FileNameTagCompleter> m_completer;
	bool m_textSelected;
	std::unique_ptr<QListWidget> m_fileNameListWidget;
};

} //namespace gloperate_qtwidgets
