#ifndef _AV_TRANSCODER_MEDIA_PROPERTY_VIDEO_PROPERTIES_HPP
#define _AV_TRANSCODER_MEDIA_PROPERTY_VIDEO_PROPERTIES_HPP

#include <AvTranscoder/common.hpp>
#include "Pixel.hpp"
#include <AvTranscoder/mediaProperty/util.hpp>
#include <AvTranscoder/file/util.hpp>
#include <AvTranscoder/progress/IProgress.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include <string>
#include <vector>
#include <utility>

namespace avtranscoder
{

class AvExport VideoProperties
{
public:
	VideoProperties( const AVFormatContext* formatContext, const size_t index, IProgress& progress, const EAnalyseLevel level = eAnalyseLevelFirstGop );

	std::string getCodecName() const;
	std::string getCodecLongName() const;
	std::string getProfileName() const;
	std::string getColorTransfert() const;
	std::string getColorspace() const;
	std::string getColorRange() const;
	std::string getColorPrimaries() const;
	std::string getChromaSampleLocation() const;
	std::string getFieldOrder() const;

	Pixel& getPixel() { return _pixel; }

	int64_t getStartTimecode() const;
	std::string getStartTimecodeString() const;

	Rational getTimeBase() const;
	Rational getSar() const; // sample/pixel aspect ratio
	Rational getDar() const; // display aspect ratio

	size_t getStreamId() const { return _streamId; }
	size_t getCodecId() const;
	size_t getBitRate() const;  ///< in bits/s
	size_t getMaxBitRate() const;
	size_t getMinBitRate() const;
	size_t getTicksPerFrame() const;
	size_t getWidth() const;
	size_t getHeight() const;
	size_t getGopSize() const;
	size_t getDtgActiveFormat() const;
	size_t getReferencesFrames() const;
	int getProfile() const;
	int getLevel() const;

	double getFps() const;

	bool hasBFrames() const;

	//@{
	// Warning: Can acces these data when analyse first gop
	// @see EAnalyseLevel
	// @see analyseGopStructure
	bool isInterlaced() const { return _isInterlaced; }
	bool isTopFieldFirst() const { return _isTopFieldFirst; }
	std::vector< std::pair< char, bool > > getGopStructure() const { return _gopStructure; }
	//@}

	PropertiesMap& getMetadatas() { return _metadatas; }

#ifndef SWIG
	const AVFormatContext& getAVFormatContext() { return *_formatContext; }
	AVCodecContext& getAVCodecContext() { return *_codecContext; }
	const Pixel& getPixel() const { return _pixel; }
#endif

	PropertiesMap getPropertiesAsMap() const;  ///< Return all video properties as a map (name of property: value)

private:
	/**
	 *  @brief frame type / is key frame
	 *  @param progress: callback to get analysis progression
	 */
	void analyseGopStructure( IProgress& progress );

private:
	const AVFormatContext* _formatContext;  ///< Has link (no ownership)
	AVCodecContext* _codecContext;  ///< Has link (no ownership)
	AVCodec* _codec;  ///< Has link (no ownership)
	Pixel _pixel;

	size_t _streamId;
	//@{
	// Can acces these data when analyse first gop
	bool _isInterlaced;
	bool _isTopFieldFirst;
	std::vector< std::pair< char, bool > > _gopStructure;
	//@}
	PropertiesMap _metadatas;
};

}

#endif
