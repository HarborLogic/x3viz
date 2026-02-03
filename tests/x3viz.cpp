// common header
#include "x3viz.h"

#ifdef _WIN32
#include <Windows.h>
#include <gl/GL.h>

//WGL extensions
#ifndef WGL_EXT_swap_control
#define WGL_EXT_swap_control 1
typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef int (WINAPI* PFNWGLGETSWAPINTERVALEXTPROC) (void);
#ifdef WGL_WGLEXT_PROTOTYPES
BOOL WINAPI wglSwapIntervalEXT(int interval);
int WINAPI wglGetSwapIntervalEXT(void);
#endif
#endif /* WGL_EXT_swap_control */

#ifndef WGL_ARB_create_context
#define WGL_ARB_create_context 1
#define WGL_CONTEXT_DEBUG_BIT_ARB         0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB       0x2093
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define ERROR_INVALID_VERSION_ARB         0x2095
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);
#ifdef WGL_WGLEXT_PROTOTYPES
HGLRC WINAPI wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList);
#endif
#endif /* WGL_ARB_create_context */

// NOTE K.D: application init stubs for windows
#ifdef _WIN32 && !defined(_CONSOLE)
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	int nargs = 0;
	char** pargv = nullptr;
	LPWSTR *pwargv = CommandLineToArgvW(GetCommandLineW(), &nargs); // make sure memory is allocated
	if (!pwargv) {
		MessageBoxA(nullptr, "Failed to parse command line arguments", "Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	pargv = new (std::nothrow)char* [nargs];
	if(!pargv) {
		MessageBoxA(nullptr, "Failed to allocate memory for command line arguments", "Error", MB_OK | MB_ICONERROR);
		LocalFree(pwargv);
		return -1;
	}
	/* cvt strings */
	for (int i = 0; i < nargs; i++) {
		char* &parg = pargv[i];
		parg = new (std::nothrow)char[(wcslen(pwargv[i]) * 4) + 1];
		if (!parg) {
			MessageBoxA(nullptr, "Failed to allocate memory for command line argument", "Error", MB_OK | MB_ICONERROR);
			for (int j = 0; j < i; j++) {
				delete[] pargv[j];
			}
			delete[] pargv;
			LocalFree(pwargv);
			return -1;
		}
		wcstombs_s(nullptr, parg, (wcslen(pwargv[i]) * 4) + 1, pwargv[i], _TRUNCATE);
	}

	/* run app */
	int result = x3::x3_run_app(nargs, pargv, nullptr);

	/* free memory */
	for (int i = 0; i < nargs; i++) {
		delete[] pargv[i];
	}
	delete[] pargv;
	LocalFree(pwargv);
	return result;
}
#else
int main(int argc, char** argv) {
	return x3::x3_run_app(argc, argv, nullptr);
}

#endif
#endif
