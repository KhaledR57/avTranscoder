#include "GeneratorVideo.hpp"

#include <AvTranscoder/essenceTransform/VideoEssenceTransform.hpp>

namespace avtranscoder
{

GeneratorVideo::GeneratorVideo( )
	: IInputEssence( )
	, _inputFrame( NULL )
	, _videoDesc()
	, _videoFrameDesc()
	, _numberOfView( 1 )
{
}

GeneratorVideo::~GeneratorVideo( )
{
}

void GeneratorVideo::setVideoDesc( const VideoCodec& videoDesc )
{
	_videoDesc = videoDesc;
	_videoFrameDesc = _videoDesc.getVideoFrameDesc();
}

VideoCodec GeneratorVideo::getVideoCodec() const
{
	return _videoDesc;
}

void GeneratorVideo::setFrame( Frame& inputFrame )
{
	_inputFrame = &inputFrame;
}

bool GeneratorVideo::readNextFrame( Frame& frameBuffer )
{
	if( ! _inputFrame )
	{
		// @todo support PAL (0 to 255) and NTFS (16 to 235)
		int fillChar = 0;

		if( frameBuffer.getSize() != _videoFrameDesc.getDataSize() )
			frameBuffer.getBuffer().resize( _videoFrameDesc.getDataSize() );

		VideoFrameDesc desc( _videoDesc.getVideoFrameDesc() );
		Pixel rgbPixel;
		rgbPixel.setColorComponents( eComponentRgb );
		rgbPixel.setPlanar( false );
		desc.setPixel( rgbPixel );

		VideoFrame intermediateBuffer( desc );
		intermediateBuffer.getBuffer().resize( _videoFrameDesc.getDataSize() );
		memset( intermediateBuffer.getPtr(), fillChar, _videoFrameDesc.getDataSize() );

		VideoEssenceTransform videoEssenceTransform;
		videoEssenceTransform.convert( intermediateBuffer, frameBuffer );

		return true;
	}
	
	if( frameBuffer.getSize() != _inputFrame->getSize() )
		frameBuffer.getBuffer().resize( _inputFrame->getSize() );
	std::memcpy( frameBuffer.getPtr(), _inputFrame->getPtr(), _inputFrame->getSize() );
	return true;
}

bool GeneratorVideo::readNextFrame( Frame& frameBuffer, const size_t subStreamIndex )
{
	return false;
}

}
