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