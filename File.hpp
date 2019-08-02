#ifndef ___INANITY_FILE_HPP___
#define ___INANITY_FILE_HPP___

#include "Storage.hpp"
#include "meta/decl.hpp"

BEGIN_INANITY

/// Abstract file class.
/** File represents a continious region of address space. */
class File : public Storage
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
	ptr<File> SliceFrom(size_t offset);

	/// Concatenate files.
	ptr<File> Concat(ptr<File> other);

	/// Storage's methods.
	bigsize_t GetBigSize() const final override;
	void Read(bigsize_t offset, size_t size, void* data) override;
	ptr<InputStream> GetInputStream(bigsize_t offset, bigsize_t size) override;

	META_DECLARE_CLASS(File);
};

END_INANITY

#endif
