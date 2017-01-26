
#pragma once


#include <string>
#include <vector>

#include <gloperate/base/EnvironmentUser.h>

// Include Component<> specialization for downstream plugins
#include <gloperate/base/Component.h>


namespace gloperate
{


/**
*  @brief
*    Loader base class
*/
class GLOPERATE_API AbstractLoader : public EnvironmentUser
{
public:
    /**
    *  @brief
    *    Constructor
    */
    explicit AbstractLoader(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractLoader();

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
};


} // namespace gloperate
