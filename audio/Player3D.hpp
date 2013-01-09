#ifndef ___INANITY_AUDIO_PLAYER_3D_HPP___
#define ___INANITY_AUDIO_PLAYER_3D_HPP___

#include "Player.hpp"
#include "../math/basic.hpp"

BEGIN_INANITY_AUDIO

/// Абстрактный класс плеера с возможностью 3D-позиционирования.
class Player3D : public Player
{
public:
	virtual void SetPosition(const float3& position) = 0;
	virtual void SetVelocity(const float3& velocity) = 0;
};

END_INANITY_AUDIO

#endif
