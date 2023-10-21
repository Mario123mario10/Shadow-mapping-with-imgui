#pragma once

#include <memory>
#include <string>

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

// Olek
class Texture2D : public Texture {
	int width;
	int height;
	int internalFormat;
	std::unique_ptr<unsigned char> textureImage;
public:
	Texture2D(int width, int height, int internalFormat, int textureLevel = 1);
	~Texture2D();
	//void loadImage(std::string imagePath);
	static std::shared_ptr<Texture2D> fromImage(std::string imagePath);
};

// Mariusz
class TextureCubeMap : public Texture {
public:
	TextureCubeMap();
};