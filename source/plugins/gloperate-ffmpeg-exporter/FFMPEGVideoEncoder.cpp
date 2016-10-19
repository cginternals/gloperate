
#include "FFMPEGVideoEncoder.h"

#include <globjects/base/baselogging.h>

extern "C" {
    #include <libavutil/opt.h>
    #include <libavcodec/avcodec.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/common.h>
    #include <libavutil/imgutils.h>
    #include <libavutil/mathematics.h>
    #include <libavutil/samplefmt.h>

    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <libavutil/pixdesc.h>
    #include <libavutil/pixfmt.h>
    #include <libavutil/samplefmt.h>
    #include <libavutil/intreadwrite.h>
}


#if LIBAVCODEC_VERSION_INT < AV_VERSION_INT(55,28,1)
#define av_frame_alloc  avcodec_alloc_frame
#endif


using namespace globjects;


FFMPEGVideoEncoder::FFMPEGVideoEncoder()
: m_context(nullptr)
, m_videoStream(nullptr)
, m_frame(nullptr)
, m_frameCounter(0)
{
    // Register codecs and formats
    avcodec_register_all();
    av_register_all();
}

FFMPEGVideoEncoder::~FFMPEGVideoEncoder()
{
}

bool FFMPEGVideoEncoder::initEncoding(const cppexpose::VariantMap & parameters)
{
    auto filepath = parameters.at("filepath").toString();
    auto format = parameters.at("format").toString();
    auto codec = parameters.at("codec").toString();
    auto width = parameters.at("width").toULongLong();
    auto height = parameters.at("height").toULongLong();
    auto fps = parameters.at("fps").toULongLong();

    auto gopsize = parameters.at("gopsize").toLongLong() != 0 ? parameters.at("gopsize").toLongLong() : fps * 2;
    auto bitrate = parameters.at("bitrate").toLongLong() != 0 ? parameters.at("bitrate").toLongLong() : 400000;

    if (filepath == "")
    {
        critical() << "Filepath must not be empty.";
        return false;
    }

    // Choose video format from file name
    AVOutputFormat * avFormat = av_guess_format(format.c_str(), NULL, NULL);
    if (!avFormat) {
        critical() << "Could not use given output format (" << format << ").";
        return false;
    }

    // Find video encoder
    AVCodec * avCodec = avcodec_find_encoder_by_name(codec.c_str());
    if (!avCodec) {
        critical() << "Codec (" << codec << ") not found.";
        return false;
    }

    // Create context
    m_context = avformat_alloc_context();
    if (!m_context) {
        critical() << "Could not create video context.";
        return false;
    }
    m_context->oformat = avFormat;

    // Create video stream
    m_videoStream = avformat_new_stream(m_context, avCodec);
    if (!m_videoStream) {
        critical() << "Could not alloc stream";
        return false;
    }

    // Set video stream type and options
    m_videoStream->codec->codec_type    = AVMEDIA_TYPE_VIDEO;
    m_videoStream->codec->codec_id      = avCodec->id;
    m_videoStream->codec->bit_rate      = bitrate;
    m_videoStream->codec->width         = width;
    m_videoStream->codec->height        = height;
    m_videoStream->codec->time_base.num = 1;
    m_videoStream->codec->time_base.den = fps;
    m_videoStream->codec->gop_size      = gopsize;
    m_videoStream->codec->pix_fmt       = AV_PIX_FMT_YUV420P;

    // Some formats want stream headers to be separate
    if (m_context->oformat->flags & AVFMT_GLOBALHEADER) {
        m_videoStream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
    }

    // [DEBUG] Output video stream info
    av_dump_format(m_context, 0, filepath.c_str(), 1);

    // Open codec
    if (avcodec_open2(m_videoStream->codec, avCodec, nullptr) < 0) {
        critical() << "Could not open codec (" << codec << ")";
        return false;
    }

    // Allocate picture for encoding
    m_frame = av_frame_alloc();
    if (!m_frame) {
        critical() << "Could not allocate frame";
        return false;
    }

    // Allocate frame buffer
    int size = avpicture_get_size(m_videoStream->codec->pix_fmt, m_videoStream->codec->width, m_videoStream->codec->height);
    uint8_t * picture_buf = (uint8_t *)av_malloc(size);
    if (!picture_buf) {
        critical() << "Could not allocate picture";
        av_free(m_frame);
        return false;
    }

    // Assign frame buffer to picture
    avpicture_fill((AVPicture *)m_frame, picture_buf, m_videoStream->codec->pix_fmt, m_videoStream->codec->width, m_videoStream->codec->height);

    // Output to file
    
    if (!(avFormat->flags & AVFMT_NOFILE)) {
        if (avio_open(&m_context->pb, filepath.c_str(), AVIO_FLAG_WRITE) < 0) {
            critical() << "Could not open  " << filepath;
            return false;
        }
    }

    // Write video header
    avformat_write_header(m_context, NULL);

    return true;
}

void FFMPEGVideoEncoder::putFrame(const gloperate::Image & image)
{
    if (image.format() == gl::GL_RGB)
    {
        putFrame(image.data(), image.width(), image.height());
    } else {
        critical() << "Image format not supported.";
    }
}

void FFMPEGVideoEncoder::putFrame(const char * data, int width, int height)
{
    // Put input image into picture structure
    AVPicture inputPicture;
    avpicture_fill(&inputPicture, (uint8_t*)data, AV_PIX_FMT_RGB24, width, height);

    // Convert input image to output frame
    SwsContext * converter = sws_getContext(width,                     height,                     AV_PIX_FMT_RGB24,
                                            m_videoStream->codec->width, m_videoStream->codec->height, AV_PIX_FMT_YUV420P,
                                            SWS_BICUBIC, NULL, NULL, NULL);

    sws_scale(converter, inputPicture.data, inputPicture.linesize, 0, height, m_frame->data, m_frame->linesize);
    sws_freeContext(converter);

    // Set frame info
    m_frame->width  = m_videoStream->codec->width;
    m_frame->height = m_videoStream->codec->height;
    m_frame->format = m_videoStream->codec->pix_fmt;

    // Set timestamp
    m_frame->pts = m_frameCounter++;

    // Create output packet
    AVPacket packet;
    av_init_packet(&packet);
    packet.data = nullptr;
    packet.size = 0;

    // Encode video frame
    int res = 0;
    if (m_context->oformat->flags & AVFMT_RAWPICTURE) {
        // Raw image format
        packet.flags |= AV_PKT_FLAG_KEY;
        packet.stream_index = m_videoStream->index;
        packet.data = (uint8_t *)m_frame;
        packet.size = sizeof(AVPicture);

        // Write frame
        res = av_write_frame(m_context, &packet);
    } else {
        // Encode image frame
        int got_output;
        avcodec_encode_video2(m_videoStream->codec, &packet, m_frame, &got_output);
        if (got_output) {
            // Rescale time stamps
            if (packet.pts != AV_NOPTS_VALUE) {
                packet.pts = av_rescale_q(packet.pts, m_videoStream->codec->time_base, m_videoStream->time_base);
            }
            if (packet.dts != AV_NOPTS_VALUE) {
                packet.dts = av_rescale_q(packet.dts, m_videoStream->codec->time_base, m_videoStream->time_base);
            }
            packet.stream_index = m_videoStream->index;

            // Write frame
            res = av_write_frame(m_context, &packet);
        }
    }

    // Check for errors
    if (res != 0) {
        critical() << "Error while writing video frame";
        return;
    }

    // Destroy packet
    av_free_packet(&packet);
}

void FFMPEGVideoEncoder::finishEncoding()
{
    // Write end of video file
    av_write_trailer(m_context);

    // Close video codec
    if (m_videoStream) {
        avcodec_close(m_videoStream->codec);
    }

    // Release frame
    if (m_frame) {
        av_free(m_frame->data[0]);
        av_free(m_frame);
    }

    // Release video streams
    for (unsigned int i = 0; i < m_context->nb_streams; i++) {
        av_freep(&m_context->streams[i]->codec);
        av_freep(&m_context->streams[i]);
    }

    // Close output file
    if (!(m_context->oformat->flags & AVFMT_NOFILE)) {
        avio_close(m_context->pb);
    }

    // Release context
    av_free(m_context);
}
