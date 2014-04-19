#ifndef ___INANITY_OUTPUT_STREAM_HPP___
#define ___INANITY_OUTPUT_STREAM_HPP___

#include "Object.hpp"
#include "meta/decl.hpp"

BEGIN_INANITY

class File;
class InputStream;

/// Abstract output stream class.
/** Derived class should redefine at least on of `Write` methods. */
class OutputStream : public Object
{
public:
	/// Write data to the stream.
	/** Stream should always write all given data.
	If it cannot (because of end of stream or for another reason),
	exception will be thrown.
	Default implementation copies data into memory file,
	and calls Write(file).
	*/
	virtual void Write(const void* data, size_t size);

	/// Write file with data to stream.
	/** It may be more effective to use this method.
	Default implementation just calls Write(data, size). */
	virtual void Write(ptr<File> file);

	/// Read all data from input stream to itself.
	/** Just convenient method. */
	bigsize_t ReadAllFromStream(ptr<InputStream> inputStream);

	META_DECLARE_CLASS(OutputStream);
};

END_INANITY

#endif
