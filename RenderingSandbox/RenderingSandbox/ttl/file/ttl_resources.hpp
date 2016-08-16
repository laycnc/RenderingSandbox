#pragma once

#include <ttl/ttl_common.hpp>
#include <ttl/file/ttl_file.hpp>

namespace ttl::file
{

	enum class resources_flag
	{
		LoadFile = 1 << 0
	};


	template<class T = byte_t>
	class resources
	{

	public:
		constexpr resources() noexcept = default;

		constexpr resources(const void* _data, size_t _size) noexcept
			: m_data(static_cast<const byte_t*>(_data))
			, m_size(_size)
			, m_flags{}
		{
		}

		resources(const char* filename)
		{
			if (auto result = LoadFile(filename))
			{
				this->m_flags = resources_flag::LoadFile;
				this->m_data = result.value().first;
				this->m_size = result.value().second;
			}
		}
		resources(const resources& _other)
			: m_data(_other.m_data)
			, m_size(_other.m_size)
			, m_flags(0)
		{
			// ポインタとサイズだけコピー
			// Note: 元のリソースが解放された場合にアクセス不能になるので注意
		}

		virtual ~resources()
		{
			this->clear();
		}

		resources& operator=(const resources& res)
		{
			// ポインタとサイズだけコピー
			// Note: 元のリソースが解放された場合にアクセス不能になるので注意
			m_data = nullptr;
			m_size = 0;
			m_flags = {};
			return *this;
		}

		void clear()
		{
			if (CheckFlag(this->m_flags, resources_flag::LoadFile))
			{	// このクラスで読み込まれているので、メモリを解放する
				delete[] m_data;
			}
			m_data = nullptr;
			m_size = 0;
			m_flags = {};
		}

		constexpr bool is_valid() const noexcept
		{
			return (m_data != nullptr); 
		}

		explicit constexpr operator bool() const noexcept
		{
			return this->is_valid();
		}

		// Getter
		constexpr const byte_t* data() const noexcept
		{
			return reinterpret_cast<const T*>(m_data);
		}

		const T* value() const noexcept
		{
			return reinterpret_cast<const T*>(this->data());
		}


		constexpr size_t size() const noexcept
		{
			return m_size;
		}

	protected:
		const byte_t*	m_data = {};
		size_t			m_size = 0;
		resources_flag	m_flags = {};
	};

}