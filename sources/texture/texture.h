#pragma once

class Texture {
protected:
	unsigned int id;
	unsigned int type;
public:
	Texture(unsigned int type);
	unsigned int getType() const;
	void activate(unsigned int slot) const;
	virtual ~Texture() =0;
};

// Olek
class Texture2D : public Texture {
public:
	Texture2D();
	virtual ~Texture2D();
};

// Mariusz
class TextureCubeMap : public Texture {
public:
	TextureCubeMap();
	virtual ~TextureCubeMap();
};