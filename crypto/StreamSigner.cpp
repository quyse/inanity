#include "StreamSigner.hpp"
#include "HashAlgorithm.hpp"
#include "SignatureAlgorithm.hpp"
#include "HashStream.hpp"
#include "StreamHasher.hpp"
#include "../MemoryFile.hpp"
#include "../MemoryStream.hpp"
#include "../StreamReader.hpp"
#include "../StreamWriter.hpp"
#include "../FileInputStream.hpp"
#include "../Exception.hpp"

StreamSigner::StreamSigner(ptr<HashAlgorithm> blockHashAlgorithm, ptr<HashAlgorithm> signatureHashAlgorithm, ptr<SignatureAlgorithm> signatureAlgorithm, size_t blockSize)
: blockHashAlgorithm(blockHashAlgorithm), signatureHashAlgorithm(signatureHashAlgorithm), signatureAlgorithm(signatureAlgorithm), blockSize(blockSize)
{
	try
	{
		// проверить совместимость алгоритмов
		if(signatureAlgorithm->GetDataSize() != signatureHashAlgorithm->GetHashSize())
			THROW_PRIMARY_EXCEPTION("Signature algorithm and signature hash algorithm are not compatible");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create stream signer", exception);
	}
}

void StreamSigner::WriteSigningHeader(ptr<InputStream> sourceStream, ptr<OutputStream> destStream, ptr<File> privateKey)
{
	try
	{
		// проверить, что закрытый ключ имеет правильный размер
		if(privateKey->GetSize() != signatureAlgorithm->GetPrivateKeySize())
			THROW_PRIMARY_EXCEPTION("Invalid private key size");

		ptr<StreamWriter> writer = NEW(StreamWriter(destStream));

		bigsize_t sourceDataSize;

		// сформировать цепочку хешей
		ptr<File> hashes;
		{
			ptr<StreamHasher> hasher = NEW(StreamHasher(blockHashAlgorithm, blockSize));
			ptr<MemoryStream> hashesStream = NEW(MemoryStream());
			ptr<StreamHasher::HasherStream> hasherStream = hasher->CreateHasherStream(hashesStream);
			sourceDataSize = hasherStream->ReadAllFromStream(sourceStream);
			hasherStream->Flush();
			hashes = hashesStream->ToFile();
		}

		// записать заголовок
		writer->WriteShortlyBig(sourceDataSize);
		writer->WriteShortly(blockSize);
		// записать хеши
		destStream->Write(hashes->GetData(), hashes->GetSize());

		// сформировать цифровую подпись хешей
		{
			// для этого получить хеш от хешей
			ptr<HashStream> hashStream = signatureHashAlgorithm->CreateHashStream();
			hashStream->Write(hashes->GetData(), hashes->GetSize());
			hashStream->End();
			ptr<MemoryFile> hashesHash = NEW(MemoryFile(hashStream->GetHashSize()));
			hashStream->GetHash(hashesHash->GetData());
			// сделать подпись
			ptr<MemoryFile> signature = NEW(MemoryFile(signatureAlgorithm->GetSignatureSize()));
			signatureAlgorithm->Sign(hashesHash->GetData(), privateKey->GetData(), signature->GetData());
			// и записать её
			destStream->Write(signature->GetData(), signature->GetSize());
		}
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't sign file with stream signer", exception);
	}
}

ptr<InputStream> StreamSigner::CreateVerifyStream(ptr<InputStream> sourceStream, ptr<File> publicKey)
{
	return NEW(VerifyStream(this, sourceStream, publicKey));
}

StreamSigner::VerifyStream::VerifyStream(ptr<StreamSigner> signer, ptr<InputStream> sourceStream, ptr<File> publicKey)
: signer(signer), sourceStream(sourceStream), publicKey(publicKey)
{
	try
	{
		// проверить, что открытый ключ имеет правильный размер
		if(publicKey->GetSize() != signer->signatureAlgorithm->GetPublicKeySize())
			THROW_PRIMARY_EXCEPTION("Invalid public key size");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create stream signer\'s verify stream", exception);
	}
}

void StreamSigner::VerifyStream::ReadHeader()
{
	try
	{
		ptr<StreamReader> reader = NEW(StreamReader(sourceStream));
		bigsize_t dataSize = reader->ReadShortlyBig();
		size_t blockSize = reader->ReadShortly();
		// вычислить количество блоков
		size_t blocksCount = (size_t)((dataSize + blockSize - 1) / blockSize);
		// считать хеши блоков
		ptr<MemoryFile> hashes = NEW(MemoryFile(blocksCount * signer->blockHashAlgorithm->GetHashSize()));
		reader->Read(hashes->GetData(), hashes->GetSize());
		// вычислить хеш от хешей
		ptr<MemoryFile> hashesHash = NEW(MemoryFile(signer->signatureHashAlgorithm->GetHashSize()));
		ptr<HashStream> hashStream = signer->signatureHashAlgorithm->CreateHashStream();
		hashStream->Write(hashes->GetData(), hashes->GetSize());
		hashStream->End();
		hashStream->GetHash(hashesHash->GetData());
		// считать цифровую подпись хешей
		ptr<MemoryFile> signature = NEW(MemoryFile(signer->signatureAlgorithm->GetSignatureSize()));
		reader->Read(signature->GetData(), signature->GetSize());
		// и проверить её
		if(!signer->signatureAlgorithm->Verify(hashesHash->GetData(), publicKey->GetData(), signature->GetData()))
			THROW_PRIMARY_EXCEPTION("Wrong hashes signature");
		// создать поток для считывания данных
		verifyStream = MakePointer(NEW(StreamHasher(signer->blockHashAlgorithm, signer->blockSize)))->CreateVerifyStream(sourceStream, NEW(FileInputStream(hashes)));
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't read signed stream\'s header", exception);
	}
}

size_t StreamSigner::VerifyStream::Read(void* data, size_t size)
{
	try
	{
		if(!verifyStream)
			ReadHeader();

		return verifyStream->Read(data, size);
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't read from signed stream", exception);
	}
}
