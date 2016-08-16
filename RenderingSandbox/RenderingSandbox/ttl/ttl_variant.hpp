#pragma once


#include <limits>
#include <cstdint>
#include <type_traits>
#include <ttl/ttl_type_traits.hpp>


namespace ttl::detail_variant
{

	template<size_t N,class ... T>
	struct variant_literal_storage;

	template<size_t N>
	struct variant_literal_storage<N>
	{
		static constexpr size_t value = N;

		constexpr variant_literal_storage() noexcept = default;
		constexpr variant_literal_storage(...) noexcept {}
	};

	template<size_t N, class T, class ... TN>
	struct variant_literal_storage<N, T, TN...>
	{
		static constexpr size_t value = N - sizeof ... (TN) - 1;
		union
		{
			T									m_value;
			variant_literal_storage<N, TN...>	m_next;
			char								___dummy;
		};

		constexpr variant_literal_storage() noexcept
			: ___dummy()
		{}

		template<class U>
		constexpr variant_literal_storage(U&& _other, std::true_type) noexcept
			: m_value(std::forward<U&&>(_other))
		{
		}

		template<class U>
		constexpr variant_literal_storage(U&& _other, std::false_type) noexcept
			: m_next(std::forward<U&&>(_other))
		{
		}

		template<class U>
		constexpr variant_literal_storage(U&& _other) noexcept
			: variant_literal_storage(std::forward<U&&>(_other), std::is_same<U, T>())
		{
		}

		template<class U>
		void assign(U&& _other, std::true_type) noexcept
		{
			this->m_value = std::forward<U&&>(_other);
		}

		template<class U>
		void assign(U&& _other, std::false_type) noexcept
		{
			this->m_next.assign(std::forward<U&&>(_other));
		}

		template<class U>
		void assign(U&& _other) noexcept
		{
			this->assign(std::forward<U&&>(_other), std::is_same<T, U>());
		}

		template<size_t N, class VariantStorage>
		friend constexpr decltype(auto) Get(VariantStorage& _self, std::true_type) noexcept;
		template<size_t N, class VariantStorage>
		friend decltype(auto) Get(VariantStorage& _self, std::false_type) noexcept;
		template<size_t N, class VariantStorage>
		friend constexpr decltype(auto) Get(VariantStorage& _self) noexcept;

	};

	template<size_t N, class VariantStorage>
	constexpr decltype(auto) Get(VariantStorage& _self, std::true_type) noexcept
	{
		return _self.m_value;
	}

	template<size_t N, class VariantStorage>
	decltype(auto) Get(VariantStorage& _self, std::false_type) noexcept
	{
		return Get<N>(_self.m_next);
	}

	template<size_t N, class VariantStorage>
	constexpr decltype(auto) Get(VariantStorage& _self) noexcept
	{
		return Get<N>(_self, std::bool_constant<N == VariantStorage::value>{});
	}


} //! namespace ttl::detail_variant

namespace ttl
{

	template<class ... T>
	class variant
	{
		static constexpr size_t invalid_v = -1;

		using storage_t = detail_variant::variant_literal_storage<sizeof...(T), T...>;

		template<class U>
		using which_of = mpl::which_of<U, T...>;

		template<class U>
		static constexpr size_t _WhichOf(mpl::identity<U>) noexcept
		{
			return which_of<U>::value;
		}
		template<class Target>
		constexpr bool __which_of(mpl::identity<Target>) const noexcept
		{
			return this->m_which == which_of<Target>::value;
		}
		storage_t	m_storage;
		size_t		m_which = invalid_v;


	public:

		/// <summary>
		/// デフォルトコンストラクタ.
		/// </summary>
		constexpr variant() noexcept
			: m_storage()
			, m_which(invalid_v)
		{}

		/// <summary>
		/// コンストラクタ.
		/// </summary>
		/// <param name="_object">オブジェクト</param>
		template<class U>
		constexpr variant(U&& _object) noexcept
			: m_storage(std::forward<U&&>(_object))
			, m_which(which_of<U>::value)
		{}

		template<class U>
		variant& assign(U&& _target) noexcept
		{
			this->m_storage.assign(std::forward<U>(_target));
			m_which = which_of<U>::value;
			return *this;
		}

		template<class U>
		variant& operator =(U&& _target) noexcept
		{
			return this->assign(std::forward<U&&>(_target));
		}

		constexpr bool invalid() const noexcept
		{
			return this->m_which == invalid_v;
		}

		constexpr explicit operator bool() const noexcept
		{
			return !this->invalid();
		}

		template<class Target, class ...T>
		friend constexpr bool which_of(const variant<T...>& _self) noexcept;

		template<class Target, class ... U>
		friend Target& get(variant<U...>& _self) noexcept;
		template<class Target, class ... U>
		friend const Target& get(const variant<U...>& _self) noexcept;

		template <class Pred, class ...T >
		friend void match(variant<T...>& _self, Pred&& _pred) noexcept;
		template <class Pred, class ...T >
		friend void match(const variant<T...>& _self, Pred&& _pred) noexcept;

	};

	template<class Target, class ...T>
	constexpr bool which_of(const variant<T...>& _self) noexcept
	{
		return _self.__which_of(mpl::identity<Target>());
	}


	template<class Target, class ... U>
	Target& get(variant<U...>& _self) noexcept
	{
		return detail_variant::Get<Target>(_self.m_storage);
	}
	template<class Target, class ... U>
	const Target& get(const variant<U...>& _self) noexcept
	{
		return detail_variant::Get<Target>(_self.m_storage);
	}


	template <class Pred, class ...T >
	void match(variant<T...>& _self, Pred&& _pred) noexcept
	{
		size_t i = 0;
		mpl::foreach([&](auto identity, auto index)
		{
			using value_type = typename decltype(identity)::type;
			if (i == index)
			{
				return _pred(get<value_type>(_self));
			}
			++i;
		});
	}


	template <class Pred, class ...T >
	void match(const variant<T...>& _self, Pred&& _pred) noexcept
	{
		size_t i = 0;
		mpl::foreach([&](auto identity, auto index)
		{
			using value_type = typename decltype(identity)::type;
			if (i == index)
			{
				return _pred(get<value_type>(_self));
			}
			++i;
		});
	}

} //! namespace ttl