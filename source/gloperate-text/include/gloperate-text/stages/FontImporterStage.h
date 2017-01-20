
#pragma once


#include <cppassist/io/FilePath.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{


class FontFace;


class GLOPERATE_TEXT_API FontImporterStage : public gloperate::Stage
{
public:
    Input<cppassist::FilePath> fontFilePath;

    Output<FontFace *> font;


public:
    explicit FontImporterStage(gloperate::Environment * environment, const std::string & name = "");


protected:
    virtual void onProcess(gloperate::AbstractGLContext * context) override;


protected:
    globjects::ref_ptr<FontFace> m_font;
};


} // namespace gloperate_text
