#pragma once

#include <ttl/ttl_common.hpp>
#include <ttl/ttl_expected.hpp>

namespace ttl::file
{


	inline expected<std::pair<byte_t*, size_t>, const char*> LoadFile(const char* filename);


} // namespace msl::detail
