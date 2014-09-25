/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <vector>
#include <string>
#include <gloperate/resources/TextureLoader.h>
#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Texture loader based on Qt
*/
class GLOPERATE_QT_API QtTextureLoader : public gloperate::TextureLoader {


    public:
        /**
        *  @brief
        *    Constructor
        */
        QtTextureLoader();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~QtTextureLoader();

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
        virtual globjects::Referenced * load(const std::string & filename) const;


    protected:
        std::vector<std::string> m_extensions; /**< List of supported file extensions (e.g., ".bmp") */
        std::vector<std::string> m_types;      /**< List of supported file types (e.g., "bmp image (*.bmp)") */


};


} // namespace gloperate_qt
