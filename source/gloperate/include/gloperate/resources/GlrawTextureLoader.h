/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/TextureLoader.h>


namespace gloperate
{


/**
*  @brief
*    Loader for glraw textures
*/
class GLOPERATE_API GlrawTextureLoader : public TextureLoader {


    public:
        /**
        *  @brief
        *    Constructor
        */
        GlrawTextureLoader();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~GlrawTextureLoader();

        /**
        *  @brief
        *    Check if this loader can load a specific file type
        *
        *  @param[in] ext
        *    File extension (e.g., '.png')
        *
        *  @return
        *    'true' if loading is implemented for given file type, else 'false'
        */
        virtual bool canLoad(const std::string & ext) const;

        /**
        *  @brief
        *    Get list of file types for loading
        *
        *  @return
        *    List of supported file types
        *
        *  @remarks
        *    Example string: "My File Type (*.mft)"
        */
        virtual std::vector<std::string> loadingTypes() const;

        /**
        *  @brief
        *    Get all file types for loading
        *
        *  @return
        *    Supported file types without description, separated by space
        *
        *  @remarks
        *    Example string: "*.mft *.any *.txt"
        */
        virtual std::string allLoadingTypes() const;

        /**
        *  @brief
        *    Load data from file
        *
        *  @param filename
        *    File name
        *
        *  @return
        *    Loaded resource (can be null)
        */
        virtual glo::Referenced * load(const std::string & filename) const;


};


} // namespace gloperate
