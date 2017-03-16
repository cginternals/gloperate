
#include <gloperate-qt/scripting/ScriptCompleter.h>

#include <cassert>

#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QAbstractItemView>

#include <cppassist/memory/make_unique.h>


namespace gloperate_qt
{


ScriptCompleter::ScriptCompleter()
: QCompleter()
, m_model(cppassist::make_unique<QStandardItemModel>())
{
    setCompletionMode(QCompleter::PopupCompletion);
    setCaseSensitivity(Qt::CaseSensitive);
    setFilterMode(Qt::MatchStartsWith);

    setModel(m_model.get());
}

ScriptCompleter::~ScriptCompleter()
{
}

void ScriptCompleter::registerWord(const QString & word)
{
    QList<QStandardItem *> items = m_model->findItems(word);
    assert(items.count() < 2);

    if (!items.isEmpty()) {
        return;
    }

    m_model->appendRow(new QStandardItem(word));
    m_model->sort(0);
}

void ScriptCompleter::deregisterWord(const QString & word)
{
    QList<QStandardItem *> items = m_model->findItems(word);
    assert(items.count() < 2);

    if (items.isEmpty()) {
        return;
    }

    m_model->removeRow(items[0]->row());
    delete items[0];
}

void ScriptCompleter::registerWords(const QStringList & words)
{
    for (const QString & word : words) {
        registerWord(word);
    }
}

void ScriptCompleter::deregisterWords(const QStringList & words)
{
    for (const QString & word : words) {
        deregisterWord(word);
    }
}


} // namespace gloperate_qt
