#ifndef ___INANITY_ARCHI_WAVEFRONT_OBJ_H___
#define ___INANITY_ARCHI_WAVEFRONT_OBJ_H___

#include "Processor.hpp"
#include "Vertex.hpp"

class WavefrontObj : public Processor
{
private:
	//конвертировать файл из входного потока данных в геометрию
	ptr<Graphics::EditableGeometry<Vertex, size_t> > Convert(std::vector<int>& positionNumbers);

public:
	String GetCommand() const;
	void PrintHelp() const;
	void Run(const std::vector<String>& arguments);
};

#endif
