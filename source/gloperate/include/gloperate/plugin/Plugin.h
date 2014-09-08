/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <string>
#include <gloperate/gloperate_api.h>


namespace gloperate
{


class Painter;


/**
*  @brief
*    Describes a plugin that can be loaded dynamically
*/
class GLOPERATE_API Plugin {


    public:
        /**
        *  @brief
        *    Constructor
        *
        *  @param[in] type
        *    Plugin type
        *  @param[in] name
        *    Name of the plugin
        *  @param[in] description
        *    Description
        *  @param[in] vendor
        *    Vendor name
        */
        Plugin(const std::string & type, const std::string & name, const std::string & description, const std::string & vendor);

        /**
        *  @brief
        *    Destructor
        */
        virtual ~Plugin();

        /**
        *  @brief
        *    Get type
        *
        *  @return
        *    Plugin type
        */
        const char * type() const;

        /**
        *  @brief
        *    Get name
        *
        *  @return
        *    Name of the plugin
        */
        const char * name() const;

        /**
        *  @brief
        *    Get description
        *
        *  @return
        *    Description
        */
        const char * description() const;

        /**
        *  @brief
        *    Get vendor
        *
        *  @return
        *    Vendor name
        */
        const char * vendor() const;

        /**
        *  @brief
        *    Create painter object
        *
        *  @return
        *    Painter
        */
        virtual Painter * createPainter() const = 0;


    protected:
        std::string m_type;        /**< Plugin type */
        std::string m_name;        /**< Name of the plugin */
        std::string m_description; /**< Description */
        std::string m_vendor;      /**< Vendor name */


};


} // namespace gloperate
