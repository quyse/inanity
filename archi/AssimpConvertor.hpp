#ifndef ___INANITY_ARCHI_ASSIMP_CONVERTOR_HPP___
#define ___INANITY_ARCHI_ASSIMP_CONVERTOR_HPP___

#include "Processor.hpp"

class AssimpConvertor : public Processor
{
public:
	//*** Processor's methods.
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
