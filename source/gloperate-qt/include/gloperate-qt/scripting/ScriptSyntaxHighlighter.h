
#pragma once


#include <QSyntaxHighlighter>
#include <QHash>
#include <QString>
#include <QTextCharFormat>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Base class for syntax highlighting
*/
class GLOPERATE_QT_API ScriptSyntaxHighlighter : public QSyntaxHighlighter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ScriptSyntaxHighlighter();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ScriptSyntaxHighlighter();

    /**
    *  @brief
    *    Add syntax pattern
    *
    *  @param[in] pattern
    *    Regular expression describing the syntax pattern
    *  @param[in] format
    *    Text format that is applied
    */
    void addPattern(const QString & pattern, const QTextCharFormat & format);

    /**
    *  @brief
    *    Add multi-line syntax pattern
    *
    *  @param[in] begin
    *    Regular expression for the beginning
    *  @param[in] end
    *    Regular expression for the end
    *  @param[in] format
    *    Text format that is applied
    */
    void addMultiLinePattern(
        const QString & begin
      , const QString & end
      , const QTextCharFormat & format);


protected:
    // Virtual QSyntaxHighlighter interface
    virtual void highlightBlock(const QString & text);


protected:
    /**
    *  @brief
    *    Descripting of a multi-line pattern
    */
    struct MultiLinePattern
    {
        MultiLinePattern()
        {
        }

        MultiLinePattern(
            const QString & begin
          , const QString & end
          , const QTextCharFormat & format)
        : begin(begin)
        , end(end)
        , format(format)
        {
        }

        QString begin;
        QString end;
        QTextCharFormat format;
    };


protected:
    QHash<QString, QTextCharFormat> m_formatsByPattern; ///< Map of formats by pattern-string
    QVector<MultiLinePattern>       m_multiLinePattern; ///< List of multi-line patterns
};


} // namespace gloperate_qt
