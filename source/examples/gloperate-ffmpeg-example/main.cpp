
#include <globjects/base/baselogging.h>

#include <gloperate/base/Image.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <random>


using namespace globjects;
using namespace gloperate_ffmpeg;


int main(int /*argc*/, char ** /*argv[]*/)
{
    VideoEncoder * encoder = new VideoEncoder();
    encoder->initEncoding("gloperate-ffmpeg-example_out.avi");

    for (int i = 0; i < 100; ++i)
    {
        // Create procedural texture
        static const int w(256);
        static const int h(256);

        gloperate::Image image(gloperate::Image::Format::RGB24, w, h);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * 3; ++i) {
            image.data()[i] = static_cast<char>(255 - static_cast<char>(r(generator) * 255));
        }

        encoder->putFrame(image);
    }

    encoder->finishEncoding();
    delete encoder;

    return 0;
}
