#pragma once

#include <type_traits>
#include <algorithm>
#include <utility>

namespace ttl::mpl::detail
{
	template<size_t N, class U, class ... T>
	struct which_of_detail
	{
		static constexpr size_t value = -1;
	};

	template<size_t N, class U, class T, class ... TN>
	struct which_of_detail<N, U, T, TN ...>
	{
		static constexpr size_t value = std::is_same<U, T>::value ? N : which_of_detail<N + 1, U, TN...>::value;
	};

}

namespace ttl::mpl
{

	/// <summary>
	/// 要素が一致した型のインデックスを取得する
	/// </summary>
	template<class U, class ... T>
	struct which_of
		: std::integral_constant<size_t, detail::which_of_detail<0, U, T...>::value>
	{
	};

	/// <summary>
	/// 要素が一致した型のインデックスを取得する
	/// </summary>
	template<class U, class ... T>
	constexpr size_t which_of_v = which_of<U, T...>::value;

	template<class T>
	struct identity
	{
		using type = T;
	};

	template<class T>
	using identity_t = typename identity<T>::type;


	template<class ... T>
	struct packer {};

}

namespace ttl::mpl::detail
{

	template<size_t N, class Pred>
	constexpr void foreach_impl(Pred&&, packer<>) noexcept {}

	template<size_t N, class Pred, class T, class ... TN>
	constexpr void foreach_impl(Pred&& _pred, packer<T, TN...>) noexcept
	{
		_pred(identity<T>(), std::integral_constant<size_t, N>());
		foreach_impl<N + 1>(std::forward<Pred&&>(_pred), packer<TN...>());
	}


}

namespace ttl::mpl
{
	template<class Pred, class ... T>
	constexpr void foreach(Pred&& _pred, packer<T...> _packer) noexcept
	{
		detail::foreach_impl<0>(std::forward<Pred&&>(_pred), _packer);
	}
}
