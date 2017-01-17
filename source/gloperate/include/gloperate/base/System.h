
#pragma once


#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Environment;


/**
*  @brief
*    System functions for scripting
*/
class GLOPERATE_API System : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment (must NOT be null!)
    */
    System(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~System();


protected:
    // Scripting functions
    void scr_load(const std::string & filename);
    std::string scr_readFile(const std::string & filename);
    void scr_writeFile(const std::string & filename, const cppexpose::Variant & value);
    void scr_appendFile(const std::string & filename, const cppexpose::Variant & value);
    void scr_exit(int exitCode);


protected:
    Environment * m_environment; ///< Gloperate environment (must NOT be null!)
};


} // namespace gloperate
