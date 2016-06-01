
#pragma once


#include <gloperate-qt/scripting/ScriptCompleter.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Text completion for JavaScript
*/
class GLOPERATE_QT_API ECMA26251Completer : public ScriptCompleter
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ECMA26251Completer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ECMA26251Completer();
};


} // namespace gloperate_qt
