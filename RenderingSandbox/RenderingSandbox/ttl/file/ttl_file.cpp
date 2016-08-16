#include "pch.h"
#include <memory>
#include <fstream>
#include "ttl/file/ttl_file.hpp"

namespace ttl::file
{

	expected<std::pair<byte_t*, size_t>, const char*> LoadFile(const char* filename)
	{
		if (auto fs = std::ifstream(filename, std::ios::binary))
		{
			fs.seekg(0, std::fstream::end);//ファイル末尾を探す
			const auto eofPos = fs.tellg();//ファイル末尾インデクスを取得
			fs.clear();//先頭にもどるために一度clear()をかける。これをしないと次のseekg()でコケるときがある。
			fs.seekg(0, std::fstream::beg);//ファイル先頭に戻る
			const auto begPos = fs.tellg();//ファイル先頭インデクスを取得

			//末尾-先頭でファイルサイズを計算
			if (const size_t size = eofPos - begPos)
			{
				byte_t *buf = new byte_t[size];//サイズ分の領域を確保
				memset(buf, 0, size);//0クリアしておく
				fs.read(reinterpret_cast<char*>(buf), size);//ファイル先頭からバッファへコピー

				return make_expected(std::make_pair(buf, size));
			}
			return make_unexpected(u8"File Size Zero byte!!");
		}

		return make_unexpected(u8"File Open Failed");

	}


}