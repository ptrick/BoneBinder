#include "Texture2D.h"
#include "Assert.h"

Texture2D::Texture2D(std::string fileName)
: mTexture(new Texture2DGLWrapper(fileName))
{
}


Texture2D::~Texture2D()
{
}

void Texture2D::bind()
{
	Assert(mTexture.get());
	if (mTexture)
		mTexture->Bind();
}

int Texture2D::getWidth() const
{
	Assert(mTexture.get());
	if (mTexture)
		return mTexture->getWidth();
	else
		return -1;
}

int Texture2D::getHeight() const
{
	Assert(mTexture.get());
	if (mTexture)
		return mTexture->getHeight();
	else
		return -1;
}
