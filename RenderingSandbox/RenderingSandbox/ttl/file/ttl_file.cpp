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
			fs.seekg(0, std::fstream::end);//�t�@�C��������T��
			const auto eofPos = fs.tellg();//�t�@�C�������C���f�N�X���擾
			fs.clear();//�擪�ɂ��ǂ邽�߂Ɉ�xclear()��������B��������Ȃ��Ǝ���seekg()�ŃR�P��Ƃ�������B
			fs.seekg(0, std::fstream::beg);//�t�@�C���擪�ɖ߂�
			const auto begPos = fs.tellg();//�t�@�C���擪�C���f�N�X���擾

			//����-�擪�Ńt�@�C���T�C�Y���v�Z
			if (const size_t size = eofPos - begPos)
			{
				byte_t *buf = new byte_t[size];//�T�C�Y���̗̈���m��
				memset(buf, 0, size);//0�N���A���Ă���
				fs.read(reinterpret_cast<char*>(buf), size);//�t�@�C���擪����o�b�t�@�փR�s�[

				return make_expected(std::make_pair(buf, size));
			}
			return make_unexpected(u8"File Size Zero byte!!");
		}

		return make_unexpected(u8"File Open Failed");

	}


}