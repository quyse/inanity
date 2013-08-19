#ifndef ___INANITY_AUDIO_PLAYER_3D_HPP___
#define ___INANITY_AUDIO_PLAYER_3D_HPP___

#include "Player.hpp"
#include "../math/basic.hpp"

BEGIN_INANITY_AUDIO

/// Abstract player class which can be positioned in 3D.
class Player3D : public Player
{
public:
	virtual void SetPosition(const Math::vec3& position) = 0;
	virtual void SetDirection(const Math::vec3& direction) = 0;
	virtual void SetVelocity(const Math::vec3& velocity) = 0;
};

END_INANITY_AUDIO

#endif
