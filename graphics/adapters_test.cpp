#include "../inanity-graphics.hpp"
#include "../inanity-dx11.hpp"
using namespace Inanity;
using namespace Inanity::Graphics;

int main()
{
	ptr<System> system = NEW(Dx11System());

	const std::vector<ptr<Adapter> >& adapters = system->GetAdapters();
	for(size_t i = 0; i < adapters.size(); ++i)
	{
		ptr<Adapter> adapter = adapters[i];
		std::cout << "Adapter\n  ID=" << adapter->GetId() << "\n  Name=" << adapter->GetName() << "\n";
		const std::vector<ptr<Monitor> >& monitors = adapter->GetMonitors();
		for(size_t j = 0; j < monitors.size(); ++j)
		{
			ptr<Monitor> monitor = monitors[i];
			std::cout << "  Monitor\n    ID=" << monitor->GetId() << "\n    Name=" << monitor->GetName() << "\n";
			const std::vector<ptr<MonitorMode> >& modes = monitor->GetModes();
			for(size_t k = 0; k < modes.size(); ++k)
			{
				ptr<MonitorMode> mode = modes[k];
				std::cout << "    Mode " << mode->GetName() << "\n";
			}
		}
	}

	return 0;
}
