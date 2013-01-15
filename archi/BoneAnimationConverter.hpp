#ifndef ___INANITY_ARCHI_BONE_ANIMATION_CONVERTER_HPP___
#define ___INANITY_ARCHI_BONE_ANIMATION_CONVERTER_HPP___

#include "Processor.hpp"

class BoneAnimationConverter : public Processor
{
public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
