
#include <gloperate/tools/AbstractVideoExporter.h>


namespace gloperate
{


AbstractVideoExporter::~AbstractVideoExporter()
{
}

int AbstractVideoExporter::progress() const
{
    return 0;
}

} // namespace gloperate
