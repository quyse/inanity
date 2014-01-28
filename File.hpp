#ifndef ___INANITY_FILE_HPP___
#define ___INANITY_FILE_HPP___

#include "Object.hpp"
#include "meta/decl.hpp"

BEGIN_INANITY

/// Abstract file class.
/** File represents a continious region of address space. */
class File : public Object
{
public:
	/// Comparer for STL classes.
	struct Comparer
	{
		bool operator()(File* a, File* b) const;
	};

public:
	virtual void* GetData() const = 0;
	virtual size_t GetSize() const = 0;

	/// Returns a part of the file.
	ptr<File> Slice(size_t offset, size_t size);
	ptr<File> SliceAt(size_t offset);

	/// Concatenate files.
	ptr<File> Concat(ptr<File> other);

	META_DECLARE_CLASS(File);
};

END_INANITY

#endif
