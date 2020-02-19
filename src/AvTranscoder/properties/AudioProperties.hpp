#ifndef _AV_TRANSCODER_MEDIA_PROPERTY_AUDIO_PROPERTIES_HPP
#define _AV_TRANSCODER_MEDIA_PROPERTY_AUDIO_PROPERTIES_HPP

#include <AvTranscoder/properties/StreamProperties.hpp>

#include <string>

namespace avtranscoder
{

class AvExport AudioProperties : public StreamProperties
{
public:
    AudioProperties(const FileProperties& fileProperties, const size_t index);

    std::string getSampleFormatName() const;
    std::string getSampleFormatLongName() const;
    std::string getChannelLayout() const;      ///< Get a description of a channel layout (example: '5.1').
    std::string getChannelName() const;        ///< Get the name of a given channel (example: 'LFE').
    std::string getChannelDescription() const; ///< Get the description of a given channel (example: 'low frequency').

    size_t getBitRate() const; ///< in bits/s, 0 if unknown
    size_t getSampleRate() const;
    size_t getNbChannels() const;
    size_t getNbSamples() const; ///< For one channel.

    size_t getTicksPerFrame() const;

#ifndef SWIG
    AVCodecContext& getAVCodecContext() { return *_codecContext; }
#endif

    PropertyVector& fillVector(PropertyVector& data) const;

private:
#ifndef SWIG
    template <typename T>
    void addProperty(PropertyVector& data, const std::string& key, T (AudioProperties::*getter)(void) const) const
    {
        try
        {
            detail::add(data, key, (this->*getter)());
        }
        catch(const std::exception& e)
        {
            detail::add(data, key, detail::propertyValueIfError);
        }
    }
#endif
};

#ifndef SWIG
AvExport std::ostream& operator<<(std::ostream& flux, const AudioProperties& audioProperties);
#endif
}

#endif
