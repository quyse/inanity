#ifndef ___INANITY_INPUT_KEY_HPP___
#define ___INANITY_INPUT_KEY_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Key numbers.
/** Cross-platform, but are get mostly from X11 key numbers. */
struct Keys
{
	enum _
	{
		_Unknown = 0,

		BackSpace = 0x08,
		Tab = 0x09,
		LineFeed = 0x0a,
		Clear = 0x0b,
		Return = 0x0d,
		Pause = 0x13,
		ScrollLock = 0x14,
		SysReq = 0x15,
		Escape = 0x1b,
		Delete = 0xff,

		Home = 0x50,
		Left = 0x51,
		Up = 0x52,
		Right = 0x53,
		Down = 0x54,
		PageUp = 0x55,
		PageDown = 0x56,
		End = 0x57,
		Begin = 0x58,

		// keypad
		NumLock = 0x7f,
		KeyPadSpace = 0x80,
		KeyPadTab = 0x89,
		KeyPadEnter = 0x8d,
		KeyPadF1 = 0x91,
		KeyPadF2 = 0x92,
		KeyPadF3 = 0x93,
		KeyPadF4 = 0x94,
		KeyPadHome = 0x95,
		KeyPadLeft = 0x96,
		KeyPadUp = 0x97,
		KeyPadRight = 0x98,
		KeyPadDown = 0x99,
		KeyPadPageUp = 0x9a,
		KeyPadPageDown = 0x9b,
		KeyPadEnd = 0x9c,
		KeyPadBegin = 0x9d,
		KeyPadInsert = 0x9e,
		KeyPadDelete = 0x9f,
		KeyPadEqual = 0xbd,
		KeyPadMultiply = 0xaa,
		KeyPadAdd = 0xab,
		KeyPadSeparator = 0xac, // comma
		KeyPadSubtract = 0xad,
		KeyPadDecimal = 0xae,
		KeyPadDivide = 0xaf,
		KeyPad0 = 0xb0,
		KeyPad1 = 0xb1,
		KeyPad2 = 0xb2,
		KeyPad3 = 0xb3,
		KeyPad4 = 0xb4,
		KeyPad5 = 0xb5,
		KeyPad6 = 0xb6,
		KeyPad7 = 0xb7,
		KeyPad8 = 0xb8,
		KeyPad9 = 0xb9,

		F1 = 0xbe,
		F2 = 0xbf,
		F3 = 0xc0,
		F4 = 0xc1,
		F5 = 0xc2,
		F6 = 0xc3,
		F7 = 0xc4,
		F8 = 0xc5,
		F9 = 0xc6,
		F10 = 0xc7,
		F11 = 0xc8,
		F12 = 0xc9,

		ShiftL = 0xe1,
		ShiftR = 0xe2,
		ControlL = 0xe3,
		ControlR = 0xe4,
		CapsLock = 0xe5,
		ShiftLock = 0xe6,
		MetaL = 0xe7,
		MetaR = 0xe8,
		AltL = 0xe9,
		AltR = 0xea,
		SuperL = 0xeb,
		SuperR = 0xec,
		HyperL = 0xed,
		HyperR = 0xee,

		Space = ' ',

		_0 = 0x30,
		_1 = 0x31,
		_2 = 0x32,
		_3 = 0x33,
		_4 = 0x34,
		_5 = 0x35,
		_6 = 0x36,
		_7 = 0x37,
		_8 = 0x38,
		_9 = 0x39,

		A = 'A',
		B = 'B',
		C = 'C',
		D = 'D',
		E = 'E',
		F = 'F',
		G = 'G',
		H = 'H',
		I = 'I',
		J = 'J',
		K = 'K',
		L = 'L',
		M = 'M',
		N = 'N',
		O = 'O',
		P = 'P',
		Q = 'Q',
		R = 'R',
		S = 'S',
		T = 'T',
		U = 'U',
		V = 'V',
		W = 'W',
		X = 'X',
		Y = 'Y',
		Z = 'Z',
	};
};
typedef Keys::_ Key;

END_INANITY_INPUT

#endif
