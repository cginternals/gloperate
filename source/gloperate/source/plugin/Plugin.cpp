/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


/**
*  @brief
*    Constructor
*/
Plugin::Plugin(const std::string & type, const std::string & name, const std::string & description, const std::string & vendor, const std::string & version)
: m_type(type)
, m_name(name)
, m_description(description)
, m_vendor(vendor)
, m_version(version)
{
}

/**
*  @brief
*    Destructor
*/
Plugin::~Plugin()
{
}

/**
*  @brief
*    Get type
*/
const char * Plugin::type() const
{
    return m_type.c_str();
}

/**
*  @brief
*    Get name
*/
const char * Plugin::name() const
{
    return m_name.c_str();
}

/**
*  @brief
*    Get description
*/
const char * Plugin::description() const
{
    return m_description.c_str();
}

/**
*  @brief
*    Get vendor
*/
const char * Plugin::vendor() const
{
    return m_vendor.c_str();
}

/**
*  @brief
*    Get version
*/
const char * Plugin::version() const
{
	return m_version.c_str();
}


} // namespace gloperate
