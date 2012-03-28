#include "HashStream.hpp"
#include <vector>
#include <sstream>
#include <iomanip>

String HashStream::GetHashString() const
{
	size_t size = GetHashSize();
	std::vector<unsigned char> hash(size);
	GetHash(&*hash.begin());
	std::ostringstream stream;
	stream << std::setfill('0') << std::right << std::hex;
	for(size_t i = 0; i < size; ++i)
		stream << std::setw(2) << (int)hash[i];
	return stream.str();
}
