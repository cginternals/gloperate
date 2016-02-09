//
//#include "TextPreparationStage.h"
//
//#include <glm/gtc/random.hpp>
//
//#include <gloperate/painter/AbstractViewportCapability.h>
//
//#include <gloperate-text/Label.h>
//#include <gloperate-text/FontFace.h>
//
//#include "TextGenerationAlgorithm.h"
//
//
//TextPreparationStage::TextPreparationStage()
//: AbstractStage("TextPreparationStage")
//{
//    addInput("font", font);
//    addInput("text", text);
//    addInput("viewport", viewport);
//
//    addOutput("label", label);
//}
//
//TextPreparationStage::~TextPreparationStage()
//{
//}
//
//void TextPreparationStage::initialize()
//{
//}
//
//void TextPreparationStage::process()
//{
//    m_vertices.resize(m_string.size());
//
//    cloud->setTexture(fontFace.glyphTexture());
//
//
//    label->setString(text.data());
//    label->typeset(*font.data());
//
//    invalidateOutputs();
//}
