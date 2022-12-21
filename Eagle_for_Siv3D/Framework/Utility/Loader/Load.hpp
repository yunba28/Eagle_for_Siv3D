#pragma once

#include <Siv3D/String.hpp>
#include <Siv3D/UnicodeConverter.hpp>
#include <Siv3D/MessageBox.hpp>

namespace eagle
{
	template<class Type>
	bool Load(const s3d::String& path, Type& value);

	template<class Type>
	bool Save(const s3d::String& directory, Type& value);
}
