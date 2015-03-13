
#include <gloperate-qtwidgets/FileNameTagCompleter.h>

#include <QString>
#include <QStringListModel>

namespace gloperate_qtwidgets
{

namespace
{
    const QStringList FilenameTags = QStringList() 
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
}

FileNameTagCompleter::FileNameTagCompleter(QStringList* currentCompletions)
:	m_currentCompletions(currentCompletions)
{
    registerWords(FilenameTags);
};

FileNameTagCompleter::~FileNameTagCompleter()
{
}

void FileNameTagCompleter::setCurrentCompletions(const QStringList& newCompletions)
{
    m_currentCompletions->clear();
    m_currentCompletions->append(newCompletions);
    emit currentCompletionsChanged();
}

QStringList FileNameTagCompleter::currentCompletions()
{
    return *m_currentCompletions;
}

bool FileNameTagCompleter::hasCompletions()
{
    return m_currentCompletions->count() > 0;
}

} // namespace gloperate_qtwidgets
