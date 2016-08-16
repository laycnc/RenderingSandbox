#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace ttl
{

	/// <summary>
	/// バイトサイズのデータを示す型.
	/// </summary>
	enum class byte_t : std::uint8_t {};


	/// <summary>
	/// 安全な削除
	/// </summary>
	/// <typeparam name="T">ポインタを示す型</typeparam>
	/// <param name="_pointer">ポインタ</param>
	template<class T>
	void SafeDelete(T*& _pointer)
	{
		if (_pointer)
		{
			delete(_pointer);
			_pointer = nullptr;
		}
	}

	/// <summary>
	/// 安全な開放
	/// </summary>
	/// <typeparam name="T">ポインタを示す型</typeparam>
	/// <param name="_pointer">ポインタ</param>
	template<class T>
	void SafeRelease(T*& _pointer)
	{
		if (_pointer)
		{
			_pointer->Release();
			_pointer = nullptr;
		}
	}


	//! フラグを立てる
	template<typename T, std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
	inline void EnableFlag(T& all, const T& flag)
	{
		all |= flag; 
	}

	template<typename T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
	inline bool EnableFlag(T& all, const T& flag)
	{
		using underlying_type = std::underlying_type_t<T>;
		EnableFlag(reinterpret_cast<underlying_type&>(all), reinterpret_cast<const underlying_type&>(flag));
	}

	//! フラグを下ろす
	template<typename T, std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
	inline void DisableFlag(T& all, const T& flag)
	{
		all &= ~flag; 
	}

	template<typename T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
	inline bool DisableFlag(T& all, const T& flag)
	{
		using underlying_type = std::underlying_type_t<T>;
		DisableFlag(reinterpret_cast<underlying_type&>(all), reinterpret_cast<const underlying_type&>(flag));
	}

	//! フラグを調べる
	template<typename T, std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
	inline constexpr bool CheckFlag(const T& all, const T& flag) noexcept
	{
		return ((all & flag) != 0);
	}

	//! フラグを調べる
	template<typename T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
	inline constexpr bool CheckFlag(const T& all, const T& flag) noexcept
	{
		return CheckFlag(static_cast<std::underlying_type_t<T>>(all), static_cast<std::underlying_type_t<T>>(flag));
	}



}