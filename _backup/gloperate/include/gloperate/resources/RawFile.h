
#pragma once


#include <cstdint>
#include <string>
#include <vector>
#include <map>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


/**
*  @brief
*    Raw data file loader
*
*    This class allows for loading raw data (e.g., textures) from files. You have to exactly know
*    how to interpret the content of the file, e.g., you need to know the format and size of the
*    texture, the file does not provide this information. To create raw textures, you can use
*    for example glraw.
*/
class GLOPERATE_API RawFile
{
public:
    RawFile(const std::string & filePath);
    virtual ~RawFile();

    const char * data() const;
    size_t size() const;

    bool isValid() const;
    const std::string & filePath() const;


protected:
    bool readFile();
    void readRawData(std::ifstream & ifs);


protected:
    const std::string m_filePath;
    std::vector<char> m_data;
    bool              m_valid;
};


} // namespace gloperate
