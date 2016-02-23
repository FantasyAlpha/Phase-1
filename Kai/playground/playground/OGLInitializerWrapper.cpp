#include "WindowManager.h"


LRESULT CALLBACK FakeCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{	
	return DefWindowProc(window, message, wParam, lParam);
}

bool InitGlew(HINSTANCE hInstance)
{
	if (!RegisterWindowClass(hInstance, "Fake window class", FakeCallback))
	{
		return false;
	}

	HWND fakeWindow = CreateWindow("Fake window class"
		, "FAKE"
		, WS_OVERLAPPEDWINDOW
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, CW_USEDEFAULT
		, NULL
		, NULL
		, hInstance
		, NULL);
	if (!fakeWindow)
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int pixelFormat = ChoosePixelFormat(GetDC(fakeWindow), &pfd);
	SetPixelFormat(GetDC(fakeWindow), pixelFormat, &pfd);

	HGLRC openglContext = wglCreateContext(GetDC(fakeWindow));
	wglMakeCurrent(GetDC(fakeWindow), openglContext);

	//NOTE(kai): Since GLEW obtains information about the supported extensions (opengl features) from the graphics driver 
	//,however some of the experimental or prerelease drivers might not report these extension
	//, so by using the glewExperimental switch we can obtain these hidden extensions 
	glewExperimental = true;

	if (glewInit() != GLEW_OK)
	{
		OutputDebugString("Couldn't initialize glew\n");
		return false;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(openglContext);
	DestroyWindow(fakeWindow);

	UnregisterClass("Fake window class", hInstance);

	return true;
}

void CorrectOpenglVersion(int *versionMajor, int *versionMinor)
{
	if (*versionMajor > 4)
	{
		*versionMajor = 4;
	}

	if (*versionMajor < 1)
	{
		*versionMajor = 1;
	}

	if (*versionMinor < 0)
	{
		versionMinor = 0;
	}

	if (*versionMinor > 5 && *versionMajor == 4)
	{
		*versionMinor = 5;
	}

	if (*versionMinor > 3 && *versionMajor == 3)
	{
		*versionMinor = 3;
	}

	if (*versionMinor > 1 && *versionMajor == 2)
	{
		*versionMinor = 1;
	}

	if (*versionMinor > 5 && *versionMajor == 1)
	{
		*versionMinor = 5;
	}
}

bool InitOpengl(HINSTANCE hInstance, WindowManager *window, int versionMajor, int versionMinor)
{
	if (!InitGlew(hInstance))
	{
		OutputDebugString("Couldn't initialize glew\n");
		return false;
	}
	HDC windowDC = GetDC(window->Window);

	CorrectOpenglVersion(&versionMajor, &versionMinor);

	PIXELFORMATDESCRIPTOR pfd;
	if (versionMajor <= 2)
	{
		pfd =
		{
			sizeof(PIXELFORMATDESCRIPTOR),
			1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
			PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
			32,                        //Colordepth of the framebuffer.
			0, 0, 0, 0, 0, 0,
			0,
			0,
			0,
			0, 0, 0, 0,
			32,                        //Number of bits for the depthbuffer
			8,                        //Number of bits for the stencilbuffer
			0,                        //Number of Aux buffers in the framebuffer.
			PFD_MAIN_PLANE,
			0,
			0, 0, 0
		};

		int pixelFormat = ChoosePixelFormat(windowDC, &pfd);

		if (!SetPixelFormat(windowDC, pixelFormat, &pfd))
		{
			OutputDebugString("Failed to set pixel format\n");
			return false;
		}
		HGLRC openglRC = {};
		openglRC = wglCreateContext(windowDC);
		wglMakeCurrent(windowDC, openglRC);

		if (!openglRC)
		{
			OutputDebugString("Failed to create opengl context\n");
			return false;
		}
	}
	else if (WGL_ARB_create_context && WGL_ARB_pixel_format)
	{
		const int pixelFormatAtrributes[] =
		{
			WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
			WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB, 32,
			WGL_DEPTH_BITS_ARB, 24,
			WGL_STENCIL_BITS_ARB, 8,
			0 // End of attributes list
		};

		int contextAttributes[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,
			WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0 // End of attributes list
		};

		int pixelFormat;
		unsigned int numFormats;

		wglChoosePixelFormatARB(windowDC, pixelFormatAtrributes, NULL, 1, &pixelFormat, &numFormats);

		if (!SetPixelFormat(windowDC, pixelFormat, &pfd))
		{
			OutputDebugString("Failed to set pixel format\n");
			return false;
		}

		HGLRC openglRC = {};
		openglRC = wglCreateContextAttribsARB(windowDC, openglRC, contextAttributes);

		bool err = wglMakeCurrent(windowDC, openglRC);

		if (!openglRC)
		{
			OutputDebugString("Failed to create opengl context\n");
			return false;
		}
		else
		{
			if (!err)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	ReleaseDC(window->Window, windowDC);



	return true;
}