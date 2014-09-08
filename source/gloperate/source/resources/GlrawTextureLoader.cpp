/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/resources/GlrawTextureLoader.h>
 

namespace gloperate
{


/**
*  @brief
*    Constructor
*/
GlrawTextureLoader::GlrawTextureLoader()
: AbstractLoader(AbstractLoader::TypeTexture)
{
}

/**
*  @brief
*    Destructor
*/
GlrawTextureLoader::~GlrawTextureLoader()
{
}

/**
*  @brief
*    Check if this loader can load a specific file type
*/
bool GlrawTextureLoader::canLoad(const std::string & ext) const
{
    return (ext == ".glraw");
}

/**
*  @brief
*    Get list of file types for loading
*/
std::vector<std::string> GlrawTextureLoader::loadingTypes() const
{
    static std::vector<std::string> fileTypes {
        "Glraw Texture (*.glraw)"
    };

    return fileTypes;
}

/**
*  @brief
*    Get all file types for loading
*/
std::string GlrawTextureLoader::allLoadingTypes() const
{
    return "*.glraw";
}

/**
*  @brief
*    Load data from file
*/
glo::Referenced * GlrawTextureLoader::load(const std::string & /*filename*/) const
{
    return nullptr;
}


} // namespace gloperate
