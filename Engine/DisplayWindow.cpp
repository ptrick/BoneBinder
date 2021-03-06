// DisplayWindow.cpp 
// compile with: cl /EHsc DisplayWindow.cpp /link Engine.lib

#include "DisplayWindow.h"
#include <GL/glew.h>
#include <iostream>
#include "InputManager.h"

DisplayWindow::~DisplayWindow()
{
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

int DisplayWindow::GetWidth() const
{
	return mWidth;
}

int DisplayWindow::GetHeight() const
{
	return mHeight;
}

float DisplayWindow::GetApectPixelRatio() const
{
	return mWidth / (float)mHeight;
}

DisplayWindow::DisplayWindow(int width, int height, const std::string &title, InputManager &inputManager)
: mIsClosed(false),
mWidth(width),
mHeight(height),
mInputManager(inputManager)
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	mWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_OPENGL);
	mGLContext = SDL_GL_CreateContext(mWindow);

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		ShowNotificationMessageBox("Glew failed to initialize.", MessageType::ERROR_MESSAGE_TYPE);
	}
}

void DisplayWindow::SwapBuffers()
{
	SDL_GL_SwapWindow(mWindow);
}

void DisplayWindow::Update()
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_MOUSEWHEEL:
			mInputManager.DoMouseWheelEvent(e.wheel);
			break;
		case SDL_MOUSEMOTION:
			mInputManager.DoMouseMovementEvent(e.motion);
			break;
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			mInputManager.DoMouseButtonEvent(e.button);
			break;
		case SDL_KEYDOWN:
			mInputManager.DoKeyDownEvent(e.key);
			break;
		case SDL_KEYUP:
			mInputManager.DoKeyUpEvent(e.key);
			break;
		case SDL_QUIT:
			mIsClosed = true;
			break;
		default:
			break;
		}
	}
}

Uint32 DisplayWindow::GetTicks() const
{
	return SDL_GetTicks();
}

void DisplayWindow::ShowNotificationMessageBox(std::string message, MessageType type)
{
	Uint32 flags;
	std::string title;
	if (type == INFORMATION_MESSAGE_TYPE)
	{
		flags = SDL_MESSAGEBOX_INFORMATION;
		title = "Information";
	}
	else if (type == WARNING_MESSAGE_TYPE)
	{
		flags = SDL_MESSAGEBOX_WARNING;
		title = "Warning";
	}
	else if (type == ERROR_MESSAGE_TYPE)
	{
		flags = SDL_MESSAGEBOX_ERROR;
		title = "Error";
	}
	else
		return;

	SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(), mWindow);
}

DisplayWindow DisplayWindow::operator=(const DisplayWindow &rhs)
{
	return std::move(rhs);
}
