
#include <gloperate/resources/ResourceManager.h>

#include <gloperate-qt/viewer/QtTextureLoader.h>

#include  <gloperate-text/FontImporter.h>
#include  <gloperate-text/FontFace.h>

int main(int argc, char * argv[])
{
    gloperate::ResourceManager resourceManager;

    resourceManager.addLoader(new gloperate_qt::QtTextureLoader());

    gloperate_text::FontImporter importer(resourceManager);

    gloperate_text::FontFace * face = importer.loadFont("data/gloperate-text/fonts/test.txt");

    delete face;

    return 0;
}
