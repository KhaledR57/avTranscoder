#include "VideoGenerator.hpp"

#include <AvTranscoder/util.hpp>

#include <sstream>

namespace avtranscoder
{

VideoGenerator::VideoGenerator(const VideoFrameDesc& frameDesc)
    : _inputFrame(NULL)
    , _blackImage(NULL)
    , _frameDesc(frameDesc)
{
}

VideoGenerator::~VideoGenerator()
{
    delete _blackImage;
}

bool VideoGenerator::decodeNextFrame(IFrame& frameBuffer)
{
    // Generate black image
    if(!_inputFrame)
    {
        // Generate the black image only once
        if(!_blackImage)
        {
            // Create the black RGB image
            VideoFrameDesc blackDesc(_frameDesc._width, _frameDesc._height, "rgb24");
            _blackImage = new VideoFrame(blackDesc);
            const unsigned char fillChar = 0;
            _blackImage->assignValue(fillChar);

            std::stringstream msg;
            msg << "Generate a black image with the following features:" << std::endl;
            msg << "width = " << _blackImage->getWidth() << std::endl;
            msg << "height = " << _blackImage->getHeight() << std::endl;
            msg << "pixel format = " << getPixelFormatName(_blackImage->getPixelFormat()) << std::endl;
            LOG_INFO(msg.str())

        }
        LOG_DEBUG("Copy data of the black image when decode next frame")
        // Convert the black image to the configuration of the given frame
        _videoTransform.convert(*_blackImage, frameBuffer);
    }
    // Take image from _inputFrame
    else
    {
        LOG_DEBUG("Convert data of the image specified when decode next frame")
        _videoTransform.convert(*_inputFrame, frameBuffer);
    }

    incrementNbDecodedFrames();
    return true;
}

bool VideoGenerator::decodeNextFrame(IFrame& frameBuffer, const std::vector<size_t> channelIndexArray)
{
    return false;
}
}
