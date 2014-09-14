/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/resources/TextureStorer.h>
#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


/**
*  @brief
*    Texture storer based on Qt
*/
class GLOPERATE_QT_API QtTextureStorer : public gloperate::TextureStorer {


    public:
        /**
        *  @brief
        *    Constructor
        */
        QtTextureStorer();

        /**
        *  @brief
        *    Destructor
        */
        virtual ~QtTextureStorer();

        /**
        *  @brief
        *    Check if this storer can store a specific file type
        *
        *  @param[in] ext
        *    File extension (e.g., '.png')
        *
        *  @return
        *    'true' if storing is implemented for given file type, else 'false'
        */
        virtual bool canStore(const std::string & ext) const;

        /**
        *  @brief
        *    Get list of file types for storage
        *
        *  @return
        *    List of supported file types
        *
        *  @remarks
        *    Example string: "My File Type (*.mft)"
        */
        virtual std::vector<std::string> storingTypes() const;

        /**
        *  @brief
        *    Get all file types for storage
        *
        *  @return
        *    Supported file types without description, separated by space
        *
        *  @remarks
        *    Example string: "*.mft *.any *.txt"
        */
        virtual std::string allStoringTypes() const;

        /**
        *  @brief
        *    Store data to file
        *
        *  @param filename
        *    File name
        *  @param object
        *   Storage resource
        *
        *  @return
        *   'true' if storage was successful, else 'false'
        */
        virtual bool store(const std::string & filename, const globjects::Referenced * object) const override;


};


} // namespace gloperate_qt
