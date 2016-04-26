#pragma once

#include <QCompleter>

#include <gloperate-qt/gloperate-qt_api.h>


class QStandardItemModel;
class QString;
class QStringList;


namespace gloperate_qt
{

class GLOPERATE_QT_API ScriptCompleter : public QCompleter
{
public:
    ScriptCompleter();
    virtual ~ScriptCompleter();

    void registerWord(const QString & word);
    void deregisterWord(const QString & word);

    void registerWords(const QStringList & words);
    void deregisterWords(const QStringList & words);

protected:
    QStandardItemModel * m_model;
};

} // namespace gloperate_qt
