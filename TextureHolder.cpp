#include "TextureHolder.h"
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;
TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

Texture& TextureHolder::GetTexture(string const& filename)
{
	//Get reference to m_Textures using m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	//auto here is equivalent of map<string, Texture>
	//Create iterator to hold key value pair and search for the required kvp using the passed in file name
	auto keyValuePair = m.find(filename);
	//auto here is equivalent of map<string, Texture>::iterator

	//Did we find a match?
	if (keyValuePair != m.end())
	{
		//Yes return the texture, the second part of the kvp, the texture
		return keyValuePair->second;
	}
	else
	{
		//File name not found
		//Create a new key value pair using the filename
		auto& texture = m[filename];
		//Load the texture from file in the usual way
		texture.loadFromFile(filename);
		//Return the texture to the calling code
		return texture;
	}
}