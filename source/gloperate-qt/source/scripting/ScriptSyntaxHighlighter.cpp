
#include <gloperate-qt/scripting/ScriptSyntaxHighlighter.h>

#include <cassert>

#include <QRegExp>


namespace gloperate_qt
{

ScriptSyntaxHighlighter::ScriptSyntaxHighlighter()
: QSyntaxHighlighter(static_cast<QTextDocument*>(nullptr))
{
}

ScriptSyntaxHighlighter::~ScriptSyntaxHighlighter()
{
}

void ScriptSyntaxHighlighter::addPattern(
    const QString & pattern
,   const QTextCharFormat & format)
{
    QRegExp re(pattern);
    assert(re.isValid());

    m_formatsByPattern[pattern] = format;
}

void ScriptSyntaxHighlighter::addMultiLinePattern(
    const QString & begin
,   const QString & end
,   const QTextCharFormat & format)
{
    QRegExp re;

    re.setPattern(begin);
    assert(re.isValid());
    re.setPattern(end);
    assert(re.isValid());

    m_multiLinePattern.push_back(MultiLinePattern(begin, end, format));
}

void ScriptSyntaxHighlighter::highlightBlock(const QString & text)
{
    for (const QString & pattern : m_formatsByPattern.keys())
    {
        const QTextCharFormat & format(m_formatsByPattern[pattern]);
        QRegExp re(pattern);

        int index = 0, length = 0;
        while (-1 != (index = text.indexOf(re, qMax(0, index + length))))
        {
            length = re.matchedLength();
            setFormat(index, length, format);
        }
    }

    for (const MultiLinePattern & mlp : m_multiLinePattern)
    {
        setCurrentBlockState(0);

        QRegExp re0(mlp.begin);
        QRegExp re1(mlp.end);

        int i0 = 0;
        if (1 != previousBlockState())
            i0 = text.indexOf(re0);

        while (i0 >= 0)
        {
            int i1 = text.indexOf(re1, i0);

            int length;
            if (-1 == i1)
            {
                setCurrentBlockState(1);
                length = text.length() - i0;
            }
            else
                length = i1 - i0 + re1.matchedLength();

            setFormat(i0, length, mlp.format);
            i0 = text.indexOf(re0, i0 + length);
        }
    }
}

} // namespace gloperate_qt
