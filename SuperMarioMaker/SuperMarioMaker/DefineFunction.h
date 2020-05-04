#pragma once

// SafeDelete
template<typename T>
static void SafeDelete(T*& p)
{
	if (p == nullptr) return;

	delete p;
	p = nullptr;
}

template<typename T>
static void SageDelete_Array(T*& p)
{
	if (p == nullptr) return;

	delete[] p;
	p = nullptr;
}

static bool IntersectPoint(POINT point, RECT rect)
{
	if (rect.left <= point.x && rect.right >= point.x && rect.top <= point.y && rect.bottom >= point.y)
	{
		return true;
	}

	return false;
}