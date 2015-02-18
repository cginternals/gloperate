#pragma once

#include <widgetzeug/ScriptCompleter.h>

#include <gloperate-qtwidgets/gloperate-qtwidgets_api.h>

namespace gloperate_qtwidgets
{

class GLOPERATE_QTWIDGETS_API FileNameTagCompleter : public widgetzeug::ScriptCompleter
{
public:
	FileNameTagCompleter();
	virtual ~FileNameTagCompleter();
};

} // namespace gloperate_qtwidgets
