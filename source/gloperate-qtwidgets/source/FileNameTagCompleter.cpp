
#include <gloperate-qtwidgets/FileNameTagCompleter.h>

#include <QString>

namespace gloperate_qtwidgets
{

namespace
{
    const QStringList FilenameTags = QStringList() 
        << "<width>"
        << "<height>"
        << "<enum>"
        << "<enum#"
        << "<year>"
        << "<month>"
        << "<day>"
        << "<hour>"
        << "<minute>"
        << "<second>"
        << "<millisecond>";
}

FileNameTagCompleter::FileNameTagCompleter()
{
	registerWords(FilenameTags);
};

FileNameTagCompleter::~FileNameTagCompleter()
{
}

} // namespace gloperate_qtwidgets
