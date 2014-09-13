/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


#include <gloperate/plugin/Plugin.h>


namespace gloperate
{


/**
*  @brief
*    Convenience class to describe a plugin for a painter
*/
template <typename PainterType>
class GLOPERATE_API PainterPlugin : public Plugin {


    public:
        PainterPlugin(const std::string & name, const std::string & description, const std::string & vendor, const std::string & version);
        virtual ~PainterPlugin();

        virtual Painter * createPainter() const;


};


} // namespace gloperate


#include <gloperate/plugin/PainterPlugin.hpp>
