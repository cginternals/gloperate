#pragma once

#include <widgetzeug/ScriptCompleter.h>
#include <memory>
#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

class QStringListModel;

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API FileNameTagCompleter : public widgetzeug::ScriptCompleter
{
    Q_OBJECT

public:
    FileNameTagCompleter(QStringList* currentCompletions);
    virtual ~FileNameTagCompleter();
    virtual void setCurrentCompletions(const QStringList& newCompletions);
    virtual QStringList currentCompletions();
    bool hasCompletions();
    
    static const int searchDepth{ 13 };

signals:
    void currentCompletionsChanged();

protected:
    std::unique_ptr<QStringList> m_currentCompletions;
    QStringListModel * m_model;
};

} // namespace gloperate_qtwidgets
