
#pragma once


#include <string>

#include <cppexpose/variant/Variant.h>

#include <globjects/base/Referenced.h>

#include <gloperate/base/Image.h>


class AVFormatContext;
class AVStream;
class AVFrame;


/**
*  @brief
*    Class for encoding single frames into a video using FFMPEG
*/
class FFMPEGVideoEncoder
{
public:
    /**
    *  @brief
    *    Constructor
    */
    FFMPEGVideoEncoder();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~FFMPEGVideoEncoder();

    /**
    *  @brief
    *    Initialize, and open file for video encoding
    *
    *  @param[in] filename
    *    Filename for video file output
    *  @return
    *    Boolean whether the initialization was successful or not
    */
    bool initEncoding(const cppexpose::VariantMap & parameters);

    /**
    *  @brief
    *    Put frame into video
    *
    *  @param[in] image
    *    Frame as gloperate::Image
    */
    void putFrame(const gloperate::Image & image);

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
    void putFrame(const char * data, int width, int height);

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
