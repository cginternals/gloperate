#pragma once

#include <QSyntaxHighlighter>

#include <QHash>
#include <QString>
#include <QTextCharFormat>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{

class GLOPERATE_QT_API ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    ScriptSyntaxHighlighter();
    virtual ~ScriptSyntaxHighlighter();

    void addPattern(const QString & pattern, const QTextCharFormat & format);

    void addMultiLinePattern(
        const QString & begin
    ,   const QString & end
    ,   const QTextCharFormat & format);

protected:
    virtual void highlightBlock(const QString & text);

protected:
    QHash<QString, QTextCharFormat> m_formatsByPattern;

    struct MultiLinePattern
    {
        MultiLinePattern()
        {
        }

        MultiLinePattern(
            const QString & begin
        ,   const QString & end
        ,   const QTextCharFormat & format)
        : begin(begin)
        , end(end)
        , format(format)
        {
        }
        
        QString begin;
        QString end;

        QTextCharFormat format;
    };

    QVector<MultiLinePattern> m_multiLinePattern;
};

} // namespace gloperate_qt
