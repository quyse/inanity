#include "KeyValueStore.hpp"
#include "../sqlite.hpp"
#include "../../crypto/HashStream.hpp"
#include "../../crypto/HashAlgorithm.hpp"
#include "../../StreamReader.hpp"
#include "../../StreamWriter.hpp"
#include "../../MemoryFile.hpp"
#include "../../Exception.hpp"
#include <cstring>

/*
Formats of the stream and hashes

Stream is beginned with the name of the hash algorithm
followed by a sequence of 1) key-value pairs; 2) checkpoints.
Key-value pair consists of key and value, which recorded
as length (in "shortly" format) and actual data.
Checkpoint is recorded as 0 byte (which could be read as
zero length of key in key-value pair) followed by stream hash
(without length). So, zero-length user keys are prohibited.

Sequence of pairs followed by checkpoint is called transaction.
*/

BEGIN_INANITY_OIL

const size_t KeyValueStore::sizeOfValueToStoreHash = 256;

KeyValueStore::KeyValueStore(const char* fileName, ptr<OutputStream> outputStream, ptr<Crypto::HashAlgorithm> hashAlgorithm)
: hashAlgorithm(hashAlgorithm)
{
	BEGIN_TRY();

	writer = NEW(StreamWriter(outputStream));

	hashSize = hashAlgorithm->GetHashSize();

	// initialize SQLite
	db = SqliteDb::Open(fileName);
	if(sqlite3_exec(*db,
		"CREATE TABLE IF NOT EXISTS items ("
		"id INTEGER PRIMARY KEY, "
		"key BLOB NOT NULL UNIQUE, "
		"value BLOB NOT NULL, "
		"valuehash BLOB)",
		0, 0, 0) != SQLITE_OK)
		THROW_SECONDARY("Can't create table", db->Error());
	// create statements
	stmtIs = db->CreateStatement("SELECT COUNT(*) FROM items WHERE key = ?1");
	stmtGetValueHash = db->CreateStatement("SELECT valuehash FROM items WHERE key = ?1");
	stmtGetValue = db->CreateStatement("SELECT value FROM items WHERE key = ?1");
	stmtSet = db->CreateStatement("INSERT OR REPLACE INTO items (key, value, valuehash) VALUES (?1, ?2, ?3)");
	stmtDelete = db->CreateStatement("DELETE FROM items WHERE key = ?1");

	// write a name of the hash algorithm
	writer->WriteString(hashAlgorithm->GetName());

	hashStream = hashAlgorithm->CreateHashStream();

	setHash = NEW(MemoryFile(hashSize));
	memset(setHash->GetData(), 0, hashSize);

	streamHash = NEW(MemoryFile(hashSize));
	memset(streamHash->GetData(), 0, hashSize);

	END_TRY("Can't create key value store");
}

ptr<File> KeyValueStore::TryGet(ptr<File> key) const
{
	SqliteQuery query(stmtGetValue);
	stmtGetValue->Bind(1, key);
	switch(stmtGetValue->Step())
	{
	case SQLITE_ROW:
		return stmtGetValue->ColumnBlob(0);
	case SQLITE_DONE:
		return nullptr;
	default:
		THROW("Can't try to get value from key value store");
	}
}

ptr<File> KeyValueStore::Get(ptr<File> key) const
{
	ptr<File> value = TryGet(key);
	if(!value)
		THROW("No such key in key value store");
	return value;
}

void KeyValueStore::Set(ptr<File> key, ptr<File> value)
{
	BEGIN_TRY();

	if(key->GetSize() == 0)
		THROW("Null keys are forbidden");

	const void* keyData = key->GetData();
	size_t keySize = key->GetSize();
	const void* valueData = value->GetData();
	size_t valueSize = value->GetSize();

	// calculate hashes
	void* keyHash = alloca(hashSize);
	void* valueHash = alloca(hashSize);
	hashStream->Reset();
	hashStream->Write(keyData, keySize);
	hashStream->End();
	hashStream->GetHash(keyHash);
	hashStream->Reset();
	hashStream->Write(valueData, valueSize);
	hashStream->End();
	hashStream->GetHash(valueHash);

	// update stream hash
	void* streamHashData = streamHash->GetData();
	hashStream->Reset();
	hashStream->Write(streamHashData, hashSize);
	hashStream->Write(keyHash, hashSize);
	hashStream->Write(valueHash, hashSize);
	hashStream->End();
	hashStream->GetHash(streamHashData);

	// write into stream
	writer->WriteShortly(keySize);
	writer->Write(keyData, keySize);
	writer->WriteShortly(valueSize);
	writer->Write(valueData, valueSize);

	// set into db
	if(valueSize)
	{
		SqliteQuery query(stmtSet);
		stmtSet->Bind(1, key);
		stmtSet->Bind(2, value);
		if(value->GetSize() < sizeOfValueToStoreHash)
			stmtSet->BindNull(3);
		else
			stmtSet->Bind(3, valueHash, hashSize);
		if(stmtSet->Step() != SQLITE_DONE)
			THROW_SECONDARY("Can't perform insert-or-update key-value to db", db->Error());
	}
	else
	{
		SqliteQuery query(stmtDelete);
		stmtDelete->Bind(1, key);
		if(stmtDelete->Step() != SQLITE_DONE)
			THROW_SECONDARY("Can't perform delete key-value from db", db->Error());
	}

	END_TRY("Can't set value to key value store");
}

void KeyValueStore::Checkpoint()
{
	writer->WriteShortly(0);
	writer->Write(streamHash);
}

void KeyValueStore::Consume(ptr<InputStream> inputStream)
{
	BEGIN_TRY();

	StreamReader reader(inputStream);

	// read name of the hash algorithm and compare it to ours
	if(reader.ReadString() != hashAlgorithm->GetName())
		THROW("Wrong hash algorithm");

	void* currentTransactionHash = alloca(hashSize);
	memcpy(currentTransactionHash, streamHash->GetData(), hashSize);
	void* tempKeyHash = alloca(hashSize);
	void* tempValueHash = alloca(hashSize);

	// cycle for transactions
	for(;;)
	{
		SqliteTransaction transaction(db);

		try
		{
			// cycle for records in transaction
			for(;;)
			{
				size_t keySize = reader.ReadShortly();
				// if this is a checkpoint
				if(keySize == 0)
				{
					// read hash (into temporary variable,
					// just to not to make one more variable)
					reader.Read(tempKeyHash, hashSize);
					// compare it to current hash in transaction
					if(memcmp(tempKeyHash, currentTransactionHash, hashSize) != 0)
						THROW("Hash is different");

					// no more records in transaction
					break;
				}
				// else this is a key-value pair
				else
				{
					ptr<MemoryFile> key = NEW(MemoryFile(keySize));
					reader.Read(key->GetData(), keySize);
					size_t valueSize = reader.ReadShortly();
					ptr<MemoryFile> value = NEW(MemoryFile(valueSize));
					reader.Read(value->GetData(), valueSize);

					// hash of the key
					hashStream->Reset();
					hashStream->Write(key->GetData(), keySize);
					hashStream->End();
					hashStream->GetHash(tempKeyHash);
					// hash of the value
					hashStream->Reset();
					hashStream->Write(value->GetData(), valueSize);
					hashStream->End();
					hashStream->GetHash(tempValueHash);

					// compute new stream hash
					hashStream->Reset();
					hashStream->Write(currentTransactionHash, hashSize);
					hashStream->Write(tempKeyHash, hashSize);
					hashStream->Write(tempValueHash, hashSize);
					hashStream->End();
					hashStream->GetHash(currentTransactionHash);
				}
			}

			// all right, commit transaction and move forward
			transaction.Commit();
		}
		catch(Exception* exception)
		{
			// transaction will be rollbacked

			MakePointer(exception);
			break;
		}
	}

	END_TRY("Can't consume stream for key value store");
}

END_INANITY_OIL
