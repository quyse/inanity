#ifndef ___INANITY_INPUT_KEY_HPP___
#define ___INANITY_INPUT_KEY_HPP___

#include "input.hpp"

BEGIN_INANITY_INPUT

/// Key numbers.
/** Cross-platform, but are got mostly from X11 key numbers. */
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
		Insert = 0x1c,
		Delete = 0x1d,

		Space = ' ', // 0x20

		Plus = '+', // 0x2b
		Comma = ',', // 0x2c
		Hyphen = '-', // 0x2d
		Period = '.', // 0x2e
		Slash = '/', // 0x2f

		_0 = '0', // 0x30
		_1 = '1',
		_2 = '2',
		_3 = '3',
		_4 = '4',
		_5 = '5',
		_6 = '6',
		_7 = '7',
		_8 = '8',
		_9 = '9', // 0x39

		A = 'A', // 0x41
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
		Z = 'Z', // 0x5A

		Home = 0x60,
		Left = 0x61,
		Up = 0x62,
		Right = 0x63,
		Down = 0x64,
		PageUp = 0x65,
		PageDown = 0x66,
		End = 0x67,
		Begin = 0x68,
		Grave = 0x7e,

		// keypad
		NumLock = 0x7f,
		NumPadSpace = 0x80,
		NumPadTab = 0x89,
		NumPadEnter = 0x8d,
		NumPadF1 = 0x91,
		NumPadF2 = 0x92,
		NumPadF3 = 0x93,
		NumPadF4 = 0x94,
		NumPadHome = 0x95,
		NumPadLeft = 0x96,
		NumPadUp = 0x97,
		NumPadRight = 0x98,
		NumPadDown = 0x99,
		NumPadPageUp = 0x9a,
		NumPadPageDown = 0x9b,
		NumPadEnd = 0x9c,
		NumPadBegin = 0x9d,
		NumPadInsert = 0x9e,
		NumPadDelete = 0x9f,
		NumPadEqual = 0xbd,
		NumPadMultiply = 0xaa,
		NumPadAdd = 0xab,
		NumPadSeparator = 0xac, // comma
		NumPadSubtract = 0xad,
		NumPadDecimal = 0xae,
		NumPadDivide = 0xaf,

		NumPad0 = 0xb0,
		NumPad1 = 0xb1,
		NumPad2 = 0xb2,
		NumPad3 = 0xb3,
		NumPad4 = 0xb4,
		NumPad5 = 0xb5,
		NumPad6 = 0xb6,
		NumPad7 = 0xb7,
		NumPad8 = 0xb8,
		NumPad9 = 0xb9,

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
		AltL = 0xe9,
		AltR = 0xea,
		SuperL = 0xeb,
		SuperR = 0xec,
	};
};
typedef Keys::_ Key;

END_INANITY_INPUT

#endif
