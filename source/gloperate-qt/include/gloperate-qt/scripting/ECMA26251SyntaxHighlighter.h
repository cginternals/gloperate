
#pragma once


#include <gloperate-qt/scripting/ScriptSyntaxHighlighter.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Syntax highlighting for JavaScript
*/
class GLOPERATE_QT_API ECMA26251SyntaxHighlighter : public ScriptSyntaxHighlighter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ECMA26251SyntaxHighlighter();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ECMA26251SyntaxHighlighter();
};


} // namespace gloperate_qt
