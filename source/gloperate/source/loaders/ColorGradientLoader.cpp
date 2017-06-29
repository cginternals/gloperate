
#include <gloperate/loaders/ColorGradientLoader.h>

#include <algorithm>

#include <cppassist/fs/readfile.h>
#include <cppassist/fs/FilePath.h>
#include <cppassist/string/regex.h>
#include <cppassist/logging/logging.h>

#include <cppexpose/variant/Variant.h>
#include <cppexpose/json/JSON.h>

#include <gloperate/gloperate.h>
#include <gloperate/rendering/ColorGradientList.h>
#include <gloperate/rendering/LinearColorGradient.h>


namespace gloperate
{


CPPEXPOSE_COMPONENT(ColorGradientLoader, gloperate::AbstractLoader)


ColorGradientLoader::ColorGradientLoader(Environment * environment)
: Loader<ColorGradientList>(environment)
{
    // Get list of supported file formats
    m_extensions.push_back(".json");
    m_types.push_back("JSON format (*.json)");
}

ColorGradientLoader::~ColorGradientLoader()
{
}

bool ColorGradientLoader::canLoad(const std::string & ext) const
{
    // Check if file type is supported
    return (std::count(m_extensions.begin(), m_extensions.end(), "." + ext) > 0);
}

std::vector<std::string> ColorGradientLoader::loadingTypes() const
{
    // Return list of supported file types
    return m_types;
}

std::string ColorGradientLoader::allLoadingTypes() const
{
    // Compose list of all supported file extensions
    std::string allTypes;
    for (unsigned int i = 0; i < m_extensions.size(); ++i) {
        if (i > 0) allTypes += " ";
        allTypes += "*." + m_extensions[i].substr(1);
    }

    // Return supported types
    return allTypes;
}

ColorGradientList * ColorGradientLoader::load(const std::string & filename, const cppexpose::Variant &, std::function<void(int, int)> ) const
{
    ColorGradientList * colorGradientList = new ColorGradientList();

    cppexpose::Variant json;
    cppexpose::JSON reader;

    reader.load(json, filename);

    if (json.isNull())
    {
        cppassist::debug() << "Parsing of color gradient list (" << filename << ") failed.";
        return nullptr;
    }

    const cppexpose::VariantMap * jsonMap = json.asMap();

    for (const auto & gradientPair : *jsonMap)
    {
        const std::string & baseName = gradientPair.first;
        const cppexpose::VariantMap * gradient = gradientPair.second.asMap();

        for (const auto & classesPair : *gradient)
        {
            const std::string & classNumber = classesPair.first;

            if (classNumber == "type")
            {
                continue;
            }

            const cppexpose::VariantArray * colorList = classesPair.second.asArray();

            std::vector<Color> colors;
            colors.reserve(colorList->size());

            for (const auto & colorString : *colorList)
            {
                std::vector<std::string> rgbValues = cppassist::string::extract(colorString.value<std::string>(), R"([0-9]+)");

                colors.emplace_back(std::stoi(rgbValues[0]), std::stoi(rgbValues[1]), std::stoi(rgbValues[2]));
            }

            colorGradientList->add(cppassist::make_unique<LinearColorGradient>(baseName + "-" + classNumber + "", true, colors));
            colorGradientList->add(cppassist::make_unique<LinearColorGradient>(baseName + "-" + classNumber + "-continuous", false, colors));
        }
    }

    return colorGradientList;
}


} // namespace gloperate
