#pragma once


#ifdef _MSC_VER
#   define GLOPERATE_PLUGIN_API __declspec(dllexport)
#elif __GNUC__
#   define GLOPERATE_PLUGIN_API __attribute__ ((visibility ("default")))
#else
#   define GLOPERATE_PLUGIN_API
#endif
