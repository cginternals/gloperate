
#pragma once


#include <cppassist/fs/FilePath.h>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/gloperate-text_api.h>


namespace openll
{


class FontFace;


}


namespace gloperate_text
{


class GLOPERATE_TEXT_API FontImporterStage : public gloperate::Stage
{
public:
    Input<cppassist::FilePath> fontFilePath;

    Output<openll::FontFace *> font;


public:
    explicit FontImporterStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~FontImporterStage();


protected:
    virtual void onProcess() override;


protected:
    std::unique_ptr<openll::FontFace> m_font;
};


} // namespace gloperate_text
