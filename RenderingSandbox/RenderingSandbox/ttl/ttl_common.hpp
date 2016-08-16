#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace ttl
{

	/// <summary>
	/// �o�C�g�T�C�Y�̃f�[�^�������^.
	/// </summary>
	enum class byte_t : std::uint8_t {};


	/// <summary>
	/// ���S�ȍ폜
	/// </summary>
	/// <typeparam name="T">�|�C���^�������^</typeparam>
	/// <param name="_pointer">�|�C���^</param>
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
	/// ���S�ȊJ��
	/// </summary>
	/// <typeparam name="T">�|�C���^�������^</typeparam>
	/// <param name="_pointer">�|�C���^</param>
	template<class T>
	void SafeRelease(T*& _pointer)
	{
		if (_pointer)
		{
			_pointer->Release();
			_pointer = nullptr;
		}
	}


	//! �t���O�𗧂Ă�
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

	//! �t���O�����낷
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

	//! �t���O�𒲂ׂ�
	template<typename T, std::enable_if_t<!std::is_enum<T>::value>* = nullptr>
	inline constexpr bool CheckFlag(const T& all, const T& flag) noexcept
	{
		return ((all & flag) != 0);
	}

	//! �t���O�𒲂ׂ�
	template<typename T, std::enable_if_t<std::is_enum<T>::value>* = nullptr>
	inline constexpr bool CheckFlag(const T& all, const T& flag) noexcept
	{
		return CheckFlag(static_cast<std::underlying_type_t<T>>(all), static_cast<std::underlying_type_t<T>>(flag));
	}



}