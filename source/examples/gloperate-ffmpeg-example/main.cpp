
#include <globjects/base/baselogging.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <random>


using namespace globjects;
using namespace gloperate_ffmpeg;


int main(int /*argc*/, char ** /*argv[]*/)
{
    VideoEncoder * encoder = new VideoEncoder();
    encoder->createVideoEncoder("gloperate-ffmpeg-example_out.avi");

    for (int i = 0; i < 100; ++i)
    {
        debug() << i;

        // Create procedural texture
        static const int w(256);
        static const int h(256);
        char data[w * h * 3];

        std::random_device rd;
        std::mt19937 generator(rd());
        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * 3; ++i) {
            data[i] = static_cast<char>(255 - static_cast<char>(r(generator) * 255));
        }

        encoder->putFrame(data, 256, 256);
    }

    encoder->closeVideoEncoder();
    delete encoder;

    return 0;
}
