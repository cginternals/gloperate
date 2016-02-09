//
//#pragma once
//
//#include <string>
//
//#include <gloperate/pipeline/AbstractStage.h>
//#include <gloperate/pipeline/InputSlot.h>
//#include <gloperate/pipeline/Data.h>
//
//#include <gloperate-text/Label.h>
//
//
//namespace gloperate
//{
//    class AbstractViewportCapability;
//}
//
//namespace gloperate_text
//{
//    class FontFace;
//}
//
//
//class TextPreparationStage : public gloperate::AbstractStage
//{
//public:
//    TextPreparationStage();
//    virtual ~TextPreparationStage();
//
//    virtual void initialize() override;
//    virtual void process() override;
//
//public:
//    gloperate::InputSlot<gloperate_text::FontFace *> font;
//    gloperate::InputSlot<std::u32string> text;
//
//    gloperate::InputSlot<gloperate::AbstractViewportCapability *> viewport;
//
//    gloperate::Data<gloperate_text::Label> label;
//};
