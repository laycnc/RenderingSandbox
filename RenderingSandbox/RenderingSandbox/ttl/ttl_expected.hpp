#pragma once

#include <ttl/ttl_variant.hpp>

namespace ttl
{

	template<class T>
	struct unexpected_type
	{
		T m_value;
	};

	template<class T>
	struct expected_type
	{
		T m_value;
	};


	template<class T, class Error>
	class expected
	{
		variant<T, Error> m_storage;

	public:
		T& value() noexcept
		{
			return get<T>(m_storage);
		}

		explicit constexpr operator bool() const noexcept
		{
			return which_of<Error>(m_storage);
		}

		const T& value() const noexcept
		{
			return get<T>(m_storage);
		}
		const Error& message() const noexcept
		{
			return get<Error>(m_storage);
		}

		constexpr expected(expected_type<T>&& _expected) noexcept
			: m_storage(std::forward<T&&>(_expected.m_value))
		{}

		constexpr expected(unexpected_type<Error>&& _expected) noexcept
			: m_storage(std::forward<Error&&>(_expected.m_value))
		{}

	};

	template <class E>
	constexpr unexpected_type<std::decay_t<E>> make_unexpected(E&& v)
	{
		return{ std::forward<E&&>(v) };
	}

	template <class V>
	constexpr expected_type<std::decay_t<V>> make_expected(V&& v)
	{
		return{ std::forward<V&&>(v) };
	}


}