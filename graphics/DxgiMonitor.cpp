#include "DxgiMonitor.hpp"
#include "DxgiMonitorMode.hpp"
#include "../Exception.hpp"
#include "../Strings.hpp"
#include <sstream>

BEGIN_INANITY_GRAPHICS

DxgiMonitor::DxgiMonitor(ComPointer<IDXGIOutput> output)
: output(output), modesInitialized(false)
{
	try
	{
		if(FAILED(output->GetDesc(&desc)))
			THROW_PRIMARY_EXCEPTION("Can't get desc");
	}
	catch(Exception* exception)
	{
		THROW_SECONDARY_EXCEPTION("Can't create DXGI monitor", exception);
	}
}

String DxgiMonitor::GetId() const
{
	std::stringstream ss;
	ss << desc.Monitor;
	return ss.str();
}

String DxgiMonitor::GetName() const
{
	return Strings::Unicode2UTF8(desc.DeviceName);
}

const std::vector<ptr<MonitorMode> >& DxgiMonitor::GetModes()
{
	if(!modesInitialized)
		try
		{
			UINT modesCount;
			if(FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modesCount, NULL)))
				THROW_PRIMARY_EXCEPTION("Can't get modes count");
			std::vector<DXGI_MODE_DESC> modeDescs(modesCount);
			if(FAILED(output->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &modesCount, &*modeDescs.begin())))
				THROW_PRIMARY_EXCEPTION("Can't get modes");
			for(UINT i = 0; i < modesCount; ++i)
				modes.push_back(NEW(DxgiMonitorMode(modeDescs[i])));

			modesInitialized = true;
		}
		catch(Exception* exception)
		{
			THROW_SECONDARY_EXCEPTION("Can't get modes of DXGI monitor", exception);
		}

	return modes;
}

END_INANITY_GRAPHICS
