/******************************************************************************\
 * gloperate
 *
 * Copyright (C) 2014 Computer Graphics Systems Group at the 
 * Hasso-Plattner-Institut (HPI), Potsdam, Germany.
\******************************************************************************/
#pragma once


namespace gloperate
{


template <typename PainterType>
PainterPlugin<PainterType>::PainterPlugin(const std::string & name, const std::string & description, const std::string & vendor, const std::string & version)
: Plugin("Painter", name, description, vendor, version)
{
}

template <typename PainterType>
PainterPlugin<PainterType>::~PainterPlugin()
{
}

template <typename PainterType>
Painter * PainterPlugin<PainterType>::createPainter() const
{
    return new PainterType();
}


} // namespace gloperate
