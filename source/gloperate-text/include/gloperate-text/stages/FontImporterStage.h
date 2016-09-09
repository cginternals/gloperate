
#pragma once

#include <cppassist/io/FilePath.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>

#include <gloperate-text/gloperate-text_api.h>
#include <gloperate-text/FontFace.h>

namespace gloperate
{
    class ResourceManager;
}


namespace gloperate_text
{

	class FontLoader;

class GLOPERATE_TEXT_API FontImporterStage : public gloperate::Stage
{
public:
    FontImporterStage(gloperate::Environment * environment, const std::string & name = "FontImporterStage");
    virtual ~FontImporterStage();

    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess(gloperate::AbstractGLContext * context) override;

public:
    Input<gloperate::ResourceManager *> resourceManager;
    Input<cppassist::FilePath> fontFilePath;

    Output<FontFace *> font;

protected:
    std::unique_ptr<FontLoader> m_importer;
    globjects::ref_ptr<FontFace> m_font;
};


} // namespace gloperate_text
