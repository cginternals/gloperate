
#pragma once


#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


class Painter;
class ResourceManager;


/**
*  @brief
*    Represents a plugin that contains a Painter implementation
*/
class GLOPERATE_API AbstractPainterPlugin : public Plugin
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Plugin name
    *  @param[in] description
    *    Plugin description
    *  @param[in] vendor
    *    Vendor name
    *  @param[in] version
    *    Plugin version
    */
    AbstractPainterPlugin(
      const std::string & name
    , const std::string & description
    , const std::string & vendor
    , const std::string & version);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractPainterPlugin();

    /**
    *  @brief
    *    Create painter
    *
    *  @param[in] resourceManager
    *    Resource manager for the painter to load external data
    *
    *  @return
    *    Pointer to newly created Painter, nullptr on error
    */
    virtual Painter * createPainter(ResourceManager & resourceManager) const = 0;
};


/**
*  @brief
*    Concrete implementation of a painter plugin
*/
template <typename PainterType>
class PainterPlugin : public AbstractPainterPlugin
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] name
    *    Plugin name
    *  @param[in] description
    *    Plugin description
    *  @param[in] vendor
    *    Vendor name
    *  @param[in] version
    *    Plugin version
    */
    PainterPlugin(
      const std::string & name
    , const std::string & description
    , const std::string & vendor
    , const std::string & version);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PainterPlugin();

    // Virtual AbstractPainterPlugin interface
    virtual Painter * createPainter(ResourceManager & resourceManager) const override;
};


} // namespace gloperate


#include <gloperate/plugin/PainterPlugin.hpp>
