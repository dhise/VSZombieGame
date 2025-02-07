#pragma once
#ifndef TEXTURE_HOLDER_H
#define TEXTURE_HOLDER_H
#include <SFML/Graphics.hpp>
#include <map>
using namespace sf;
using namespace std;

class TextureHolder
{
private:
	//Map to hold pairs of String and Texture
	map<string, Texture> m_Textures;

	//A pointer of the same type of the class it and the one and only instance
	static TextureHolder* m_s_Instance;
public:
	TextureHolder();
	static Texture& GetTexture(string const& filename);
};


#endif