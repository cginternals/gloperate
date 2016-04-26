#pragma once

#include <gloperate-qt/scripting/ScriptSyntaxHighlighter.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{

class GLOPERATE_QT_API ECMA26251SyntaxHighlighter : public ScriptSyntaxHighlighter
{
public:
    ECMA26251SyntaxHighlighter();
    virtual ~ECMA26251SyntaxHighlighter();
};

} // namespace gloperate_qt
