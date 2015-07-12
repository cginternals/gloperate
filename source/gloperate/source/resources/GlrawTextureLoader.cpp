#include <gloperate/resources/GlrawTextureLoader.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdint>

#include <glbinding/gl/enum.h>

#include <globjects/Texture.h>

namespace
{

template<typename T>
T read(std::ifstream & stream)
{
	T value;
	stream.read(reinterpret_cast<char*>(&value), sizeof(value));
	return value;
}

std::string readString(std::ifstream & stream)
{
	std::stringstream ss;
	char c;

	while(stream.good())
	{
		stream.get(c);
		if(c == '\0')
			break;

		ss << c;
	}

	return ss.str();
}

enum class PropertyType : uint8_t
{
	Unknown = 0,
	Int = 1,
	Double = 2,
	String = 3
};

const uint16_t GLRawSignature = 0xC6F5;

int32_t getInt(const std::map<std::string, int32_t> & dict, const std::string & key)
{
	auto it = dict.find(key);
	if(it == dict.end())
	{
		throw std::exception();
	}
	return it->second;
}

}

namespace gloperate
{

GlrawTextureLoader::GlrawTextureLoader()
	: Loader<globjects::Texture>()
{
}

bool GlrawTextureLoader::canLoad(const std::string & ext) const
{
	return (ext == "glraw");
}

std::vector<std::string> GlrawTextureLoader::loadingTypes() const
{
	static std::vector<std::string> fileTypes{
		"Glraw Texture (*.glraw)"
	};

	return fileTypes;
}

std::string GlrawTextureLoader::allLoadingTypes() const
{
	return "*.glraw";
}

globjects::Texture * GlrawTextureLoader::load(const std::string & filename, std::function<void(int, int)>) const
{
	std::ifstream ifs(filename, std::ios::in | std::ios::binary);

	if(!ifs)
	{
		return nullptr;
	}

	uint64_t offset = 0;

	if(read<uint16_t>(ifs) != GLRawSignature)
	{
		return nullptr;
	}

	offset = read<uint64_t>(ifs);

	std::map<std::string, int32_t> meta = readProperties(ifs, offset);
	std::vector<char> data = readRawData(ifs, offset);

	return createTexture(meta, data);
}

globjects::Texture * GlrawTextureLoader::createTexture(const std::map<std::string, int32_t> & meta, const std::vector<char> & image)
{
	try
	{
		const gl::GLenum format = static_cast<gl::GLenum>(getInt(meta, "format"));
		const gl::GLenum type	= static_cast<gl::GLenum>(getInt(meta, "type"));
		const int32_t width  = getInt(meta, "width");
		const int32_t height = getInt(meta, "height");

		globjects::Texture * output = globjects::Texture::createDefault(gl::GL_TEXTURE_2D);
		output->image2D(0, format, width, height, 0, format, type, image.data());

		return output;
	}
	catch(std::exception&)
	{
		return nullptr;
	}
}

std::map<std::string, int32_t> GlrawTextureLoader::readProperties(std::ifstream & ifs, uint64_t offset)
{
	std::map<std::string, int32_t> properties;

	while(ifs.tellg() < static_cast<int64_t>(offset) && ifs.good())
	{
		PropertyType type = read<PropertyType>(ifs);
		std::string key = readString(ifs);

		switch(type)
		{
		case PropertyType::Int:
			properties[key] = read<int32_t>(ifs);
			break;

		case PropertyType::Double:
			read<double>(ifs);
			break;

		case PropertyType::String:
			readString(ifs);
			break;

		default:
			// We don't know the size of custom data types, so we can not proceed reading them.
			return properties;
		}
	}
	return properties;
}

std::vector<char> GlrawTextureLoader::readRawData(std::ifstream & ifs, uint64_t rawDataOffset)
{
	ifs.seekg(0, std::ios::end);

	size_t endPosition = ifs.tellg();
	const size_t size = endPosition - rawDataOffset;

	ifs.seekg(rawDataOffset, std::ios::beg);
	std::vector<char> data(size);

	ifs.read(data.data(), size);
	return data;
}

} // namespace gloperate
