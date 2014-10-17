#pragma once

#include <string>
#include <vector>

namespace gloperate
{
/** \brief Provides tools for retrieving filepathes within a directory (recursive and non-recursive).
*/
class DirectoryIterator
{
public:

    /** \brief Creates a list with the file paths of all files of a given path.
    */
    static void files(
        std::vector<std::string> & fpaths
    ,   const std::string & path
    ,   bool prependpath = false
    ,   bool recursive = false);

    static void files(
        std::vector<std::string> & fpaths
    ,   const std::vector<std::string> & paths
    ,   bool prependpath = false
    ,   bool recursive = false);

    /** \brief Creates a list with the file paths of all files of a given path.
    */
    static std::vector<std::string> files(
        const std::string & path
    ,   bool prependpath = false
    ,   bool recursive = false);

   static std::vector<std::string> files(
        const std::vector<std::string> & path
    ,   bool prependpath = false
    ,   bool recursive = false);

    /** \brief Returns a substring, starting at the last occurence of ".".
    */
    static std::string extension(const std::string & filename);

    /** \brief Removes trailing path seperators from path
    */
    static std::string truncate(const std::string & path);
};

} // namespace gloperate
