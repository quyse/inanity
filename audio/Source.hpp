#ifndef ___INANITY_AUDIO_SOURCE_HPP___
#define ___INANITY_AUDIO_SOURCE_HPP___

#include "Format.hpp"

BEGIN_INANITY

class File;
class InputStream;

END_INANITY

BEGIN_INANITY_AUDIO

/// Abstract class of sound source.
/** This is a source of sound data.
Sound sources are independent from sound system and devices,
they simply supply data.
Sound source should implement both methods GetData() and
CreateStream().
*/
class Source : public Object
{
public:
	virtual Format GetFormat() const = 0;
	/// Get sound length in samples.
	virtual size_t GetSamplesCount() const = 0;
	/// Get sound data size in bytes.
	size_t GetSize() const;

	/// Get sound data as a file.
	/** Default implementation is quite inefficient and uses CreateStream. */
	virtual ptr<File> GetData();
	/// Create stream to read sound data from beginning.
	/** Default implementation uses GetData. */
	virtual ptr<InputStream> CreateStream();
};

END_INANITY_AUDIO

#endif
