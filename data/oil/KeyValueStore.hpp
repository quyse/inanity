#ifndef ___INANITY_DATA_OIL_KEY_VALUE_STORE_HPP___
#define ___INANITY_DATA_OIL_KEY_VALUE_STORE_HPP___

#include "oil.hpp"
#include "../../crypto/crypto.hpp"

BEGIN_INANITY

class StreamWriter;
class File;
class MemoryFile;
class OutputStream;
class InputStream;

END_INANITY

BEGIN_INANITY_DATA

class SqliteDb;
class SqliteStatement;

END_INANITY_DATA

BEGIN_INANITY_CRYPTO

class HashStream;
class HashAlgorithm;

END_INANITY_CRYPTO

BEGIN_INANITY_OIL

/// Special secure store of keys and values.
class KeyValueStore : public Object
{
private:
	/// Size of value to store hash.
	/** Values smaller in size than that won't store
	hash in db. */
	static const size_t sizeOfValueToStoreHash;

	/// SQLite database.
	ptr<SqliteDb> db;
	//*** SQLite statements.
	ptr<SqliteStatement> stmtIs, stmtGetValueHash, stmtGetValue, stmtSet, stmtDelete;

	/// Writer to output stream.
	ptr<StreamWriter> writer;

	ptr<Crypto::HashAlgorithm> hashAlgorithm;
	size_t hashSize;
	/// One hash stream for all hash operations.
	ptr<Crypto::HashStream> hashStream;
	/// Current hash of the set.
	ptr<MemoryFile> setHash;
	/// Current hash of the store stream.
	ptr<MemoryFile> streamHash;

public:
	KeyValueStore(const char* fileName, ptr<OutputStream> outputStream, ptr<Crypto::HashAlgorithm> hashAlgorithm);

	ptr<File> TryGet(ptr<File> key) const;
	ptr<File> Get(ptr<File> key) const;
	void Set(ptr<File> key, ptr<File> value);
	/// Record a hash into stream to ensure that data is safe.
	void Checkpoint();

	/// Read maximum information from stream.
	void Consume(ptr<InputStream> inputStream);
};

END_INANITY_OIL

#endif
