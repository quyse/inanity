#ifndef ___INANITY_INPUT_KEY_HPP___
#define ___INANITY_INPUT_KEY_HPP___

BEGIN_INANITY_INPUT

/// Номера клавиш.
/** Кроссплатформенные. */
struct Keys
{
	enum _
	{
		a = 'A',
		b = 'B',
		c = 'C',
		d = 'D',
		e = 'E',
		f = 'F',
		g = 'G',
		h = 'H',
		i = 'I',
		j = 'J',
		k = 'K',
		l = 'L',
		m = 'M',
		n = 'N',
		o = 'O',
		p = 'P',
		q = 'Q',
		r = 'R',
		s = 'S',
		t = 'T',
		u = 'U',
		v = 'V',
		w = 'W',
		x = 'X',
		y = 'Y',
		z = 'Z',
		leftCtrl = 123,
		rightCtrl = 456
	};
};
typedef Keys::_ Key;

END_INANITY_INPUT

#endif
