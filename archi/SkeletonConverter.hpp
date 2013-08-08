#ifndef ___INANITY_ARCHI_SKELETON_CONVERTER_HPP___
#define ___INANITY_ARCHI_SKELETON_CONVERTER_HPP___

#include "Processor.hpp"

class SkeletonConverter : public Processor
{
public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
