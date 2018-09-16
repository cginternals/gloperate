
#pragma once


#include <cppfs/FilePath.h>

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
    Input<cppfs::FilePath>     fontFilePath;

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
