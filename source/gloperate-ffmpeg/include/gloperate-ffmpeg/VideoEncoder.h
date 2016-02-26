
#pragma once


#include <string>

#include <globjects/base/Referenced.h>

#include <gloperate/base/Image.h>

#include <gloperate-ffmpeg/gloperate-ffmpeg_api.h>


class AVFormatContext;
class AVStream;
class AVFrame;


namespace gloperate_ffmpeg
{


/**
*  @brief
*    Class for encoding single frames into a video using FFMPEG
*/
class GLOPERATE_FFMPEG_API VideoEncoder : public globjects::Referenced
{
public:
    /**
    *  @brief
    *    Constructor
    */
    VideoEncoder();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~VideoEncoder();

    /**
    *  @brief
    *    Initialize, and open file for video encoding
    *
    *  @param[in] filename
    *    Filename for video file output
    */
    void initEncoding(const std::string & filename);

    /**
    *  @brief
    *    Put frame into video
    *
    *  @param[in] image
    *    Frame as gloperate::Image
    */
    void putFrame(gloperate::Image image);

    /**
    *  @brief
    *    Put frame into video
    *
    *  @param[in] data
    *    Byte data of single frame, format RGB24
    *
    *  @param[in] width
    *    Frame pixel width
    *
    *  @param[in] height
    *    Frame pixel height
    */
    void putFrame(char * data, int width, int height);

    /**
    *  @brief
    *    Finalize encoding and close video file
    */
    void finishEncoding();


protected:
    AVFormatContext * m_context;
    AVStream        * m_videoStream;
    AVFrame         * m_frame;
    int               m_frameCounter;
};


} // namespace gloperate_ffmpeg
