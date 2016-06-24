
#include <globjects/base/baselogging.h>

#include <gloperate/base/Image.h>

#include <gloperate-ffmpeg/VideoEncoder.h>

#include <random>


using namespace globjects;
using namespace gloperate_ffmpeg;


int main(int /*argc*/, char ** /*argv[]*/)
{
    static const int w(256);
    static const int h(256);
        
    VideoEncoder * encoder = new VideoEncoder();
    encoder->initEncoding("gloperate-ffmpeg-example_out.avi", w, h, 25);

    for (int i = 0; i < 100; ++i)
    {
        // Create procedural texture
        gloperate::Image image(w, h, gl::GL_RGB, gl::GL_UNSIGNED_BYTE);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::poisson_distribution<> r(0.2);

        for (int i = 0; i < w * h * image.channels() * image.bytes(); ++i) {
            image.data()[i] = static_cast<char>(255 - static_cast<char>(r(generator) * 255));
        }

        encoder->putFrame(image);
    }

    encoder->finishEncoding();
    delete encoder;

    return 0;
}
