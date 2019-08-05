#ifndef ___INANITY_INPUT_NX_MANAGER_HPP___
#define ___INANITY_INPUT_NX_MANAGER_HPP___

#include "Manager.hpp"
#include "Controller.hpp"
#include "../platform/platform.hpp"
#include <unordered_map>

BEGIN_INANITY_PLATFORM

class NxWindow;

END_INANITY_PLATFORM

BEGIN_INANITY_INPUT

/// Input manager for Nintendo Switch.
class NxManager : public Manager
{
public:
	class NxController : public Controller
	{
		friend class NxManager;
	private:
		bool active = true;
		uint32_t buttons = 0;

	public:
		NxController(uint64_t controllerId);

		// Controller's methods.
		bool IsActive() const override;
		void RunHapticLeftRight(float left, float right) override;
		void StopHaptic() override;
	};

	typedef std::unordered_map<uint64_t, std::pair<int64_t, ptr<NxController> > > Controllers;
	Controllers controllers;

public:
	NxManager();

	// Manager's methods.
	void Update() override;
	void StartTextInput() override;
	void StopTextInput() override;
	ptr<Controller> TryGetController(uint64_t controllerId) override;
};

END_INANITY_INPUT

#endif
