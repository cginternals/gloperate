
#pragma once

#include <reflectionzeug/base/FilePath.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/AbstractStage.h>
#include <gloperate/pipeline/InputSlot.h>
#include <gloperate/pipeline/Data.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate
{
    class ResourceManager;
}


namespace gloperate_text
{


class FontFace;
class FontImporter;

class GLOPERATE_TEXT_API FontImporterStage : public gloperate::AbstractStage
{
public:
    FontImporterStage();
    virtual ~FontImporterStage();

    virtual void initialize() override;
    virtual void process() override;

public:
    gloperate::InputSlot<gloperate::ResourceManager *> resourceManager;
    gloperate::InputSlot<reflectionzeug::FilePath> fontFilePath;

    gloperate::Data<FontFace *> font;

protected:
    std::unique_ptr<FontImporter> m_importer;
    globjects::ref_ptr<FontFace> m_font;
};


} // namespace gloperate_text
