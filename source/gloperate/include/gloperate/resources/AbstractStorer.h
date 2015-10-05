
#pragma once


#include <string>
#include <vector>

#include <gloperate/gloperate_api.h>


namespace globjects 
{
    class Referenced;
}


namespace gloperate
{


/**
*  @brief
*    Loader base class
*/
class GLOPERATE_API AbstractStorer 
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractStorer();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractStorer();

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
    virtual bool canStore(const std::string & ext) const = 0;

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
    virtual std::vector<std::string> storingTypes() const = 0;

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
    virtual std::string allStoringTypes() const = 0;
};


} // namespace gloperate
