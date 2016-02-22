
#pragma once


#include <string>

#include <gloperate-ffmpeg/gloperate-ffmpeg_api.h>


class AVFormatContext;
class AVStream;
class AVFrame;


namespace gloperate_ffmpeg
{


/**
*  @brief
*    
*/
class GLOPERATE_FFMPEG_API VideoEncoder
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
    *    Create video encoder
    */
    void createVideoEncoder(const std::string & filename);

    /**
    *  @brief
    *    Put frame into video
    */
    void putFrame(char * data, int width, int height);

    /**
    *  @brief
    *    Close video encoder
    */
    void closeVideoEncoder();


protected:
    AVFormatContext * m_context;
    AVStream        * m_videoStream;
    AVFrame         * m_frame;
    int               m_frameCounter;
};


} // namespace gloperate_ffmpeg
