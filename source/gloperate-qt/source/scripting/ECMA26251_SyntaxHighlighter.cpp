
#include <gloperate-qt/scripting/ECMA26251_SyntaxHighlighter.h>

#include <QString>


namespace gloperate_qt
{

namespace 
{
    // http://www.ecma-international.org/ecma-262/5.1/

    const QString ECMA_Keywords = (
        "\\b("
        "break|"
        "case|"
        "catch|"
        "continue|"
        "debugger|"
        "default|"
        "delete|"
        "do|"
        "else|"
        "finally|"
        "for|"
        "function|"
        "if|"
        "in|"
        "instanceof|"
        "new|"
        "return|"
        "switch|"
        "this|"
        "throw|"
        "try|"
        "typeof|"
        "var|"
        "void|"
        "while|"
        "with"
        ")\\b");

    const QString ECMA_FutureReservedWords = (
        "\\b("
        "class|"
        "const|"
        "enum|"
        "export|"
        "extends|"
        "import|"
        "super"
        ")\\b");
}

ECMA26251SyntaxHighlighter::ECMA26251SyntaxHighlighter()
{
    QTextCharFormat keywordsFormat;
    keywordsFormat.setForeground(Qt::blue);

    QTextCharFormat futureReservedFormat;
    futureReservedFormat.setForeground(Qt::darkRed);

    addPattern(ECMA_Keywords, keywordsFormat);
    addPattern(ECMA_FutureReservedWords, futureReservedFormat);

    QTextCharFormat commentFormat;
    commentFormat.setForeground(Qt::darkGreen);

    addPattern("//[.]*[^\\n]*(\\n|$)", commentFormat);
    addMultiLinePattern("/\\*", "\\*/", commentFormat);
}

ECMA26251SyntaxHighlighter::~ECMA26251SyntaxHighlighter()
{
}

} // namespace gloperate_qt
