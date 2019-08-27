#include "Manifold.h"
#include <Physics\Body\Body.h>

bool Manifold::operator<(const Manifold & rhs) const
{
	if (A == rhs.A)
	{
		return B < rhs.B;
	}
	else
	{
		return A < rhs.A;
	}
}
