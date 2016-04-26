#pragma once

#include <gloperate-qt/scripting/ScriptCompleter.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{

class GLOPERATE_QT_API ECMA26251Completer : public ScriptCompleter
{
public:
    ECMA26251Completer();
    virtual ~ECMA26251Completer();
};

} // namespace gloperate_qt
