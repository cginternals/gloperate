
#pragma once


namespace gloperate_headless
{


using ProcAddress = void(*)();


ProcAddress getProcAddress(const char * name);


} // namespace gloperate_headless
