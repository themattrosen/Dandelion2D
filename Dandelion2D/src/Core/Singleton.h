#pragma once

template<typename T>
class Singleton
{
public:
	static constexpr inline T& Instance()
	{
		static T s_instance;
		return s_instance;
	}

private:
	
};
//
//template<typename T>
//T Singleton<T>::s_instance;
