#pragma once

class RenderbufferInterface {
protected:
	unsigned int id;
public:
	RenderbufferInterface();
	unsigned int getId() const;
	virtual ~RenderbufferInterface();
};

class Renderbuffer : public RenderbufferInterface {
	int width;
	int height;
public:
	Renderbuffer(int width, int height, unsigned int internalFormat);
};

class RenderbufferMultisample : public RenderbufferInterface {
	int width;
	int height;
	int samples;
public:
	RenderbufferMultisample(int width, int height, unsigned int internalFormat, unsigned int samples);
};