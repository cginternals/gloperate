/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <string>
#include <vector>
#include <gloperate/gloperate_api.h>


namespace glo {
    class Referenced;
}
namespace gloperate
{


/**
*  @brief
*    Load/saver base class
*/
class GLOPERATE_API AbstractLoader {


    public:
        /**
        *  @brief
        *    Resource type
        */
        enum ResourceType {
            TypeTexture = 0
        };


    public:
        /**
        *  @brief
        *    Constructor
        *
        *  @param[in] type
        *    Resource type
        */
        AbstractLoader(ResourceType type);

        /**
        *  @brief
        *    Destructor
        */
        virtual ~AbstractLoader();

        /**
        *  @brief
        *    Get resource type supported by this loader
        *
        *  @return
        *    Resource type
        */
        ResourceType resourceType() const;

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
        virtual bool canLoad(const std::string & ext) const = 0;

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
        virtual std::vector<std::string> loadingTypes() const = 0;

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
        virtual std::string allLoadingTypes() const = 0;

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
        virtual glo::Referenced * load(const std::string & filename) const = 0;


    protected:
        ResourceType m_resourceType;    /**< Resource type of the loader */


};


} // namespace gloperateGLOPERATE_API 
