#pragma once
#include <vector>
#include <string>
#include <string_view>

class Texture {
protected:
	unsigned int id;
	unsigned int type;
public:
	Texture(unsigned int type);
	unsigned int getType() const;
	void activate(unsigned int slot) const;
	unsigned int getId() const;
	virtual ~Texture();
};

class Texture2DMultisample : public Texture {
	int width;
	int height;
	int internalFormat;
	int samples;
public:
	Texture2DMultisample(int width, int height, int internalFormat, unsigned int samples);
	int getSamplesNumber() const;
};

class Texture2D : public Texture {
public:
	Texture2D(int width, int height);
	Texture2D(std::string_view filePath);
};

class TextureCubeMap : public Texture {
public:
	TextureCubeMap(const std::vector<std::string_view>& textureFilenames);
};

class ShadowMap : public Texture {
public:
	ShadowMap(int width, int height, int internalFormat);
};

class ShadowCubeMap : public Texture {
	ShadowCubeMap(int width, int height, int internalFormat);
};