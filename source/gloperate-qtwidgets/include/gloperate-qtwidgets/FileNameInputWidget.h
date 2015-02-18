#pragma once

#include <gloperate-qt/qt-includes-begin.h>
#include <QWidget>
#include <QRect>
#include <gloperate-qt/qt-includes-end.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class Ui_FileNameInputWidget;
class QKeyEvent;
class QCompleter;
class QString;

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API FileNameInputWidget : public QWidget
{
	Q_OBJECT

public:
	FileNameInputWidget(QCompleter * completer, QWidget * parent = nullptr);
	virtual ~FileNameInputWidget();

signals:
	void setCompletionPrefix(const QString & prefix);

protected:
	void correctCompleterPrefix(int oldPos, int newPos);

protected:
	QCompleter * m_completer;

private:
	const QScopedPointer<Ui_FileNameInputWidget> m_ui;
};

} //namespace gloperate_qtwidgets
