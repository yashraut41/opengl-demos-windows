#include<Windows.h>
#include<gl/glew.h>
#include<gl/GL.h>
#include<stdio.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

FILE* gpFile; //File pointer for log file
HDC ghdc = NULL; //Handle to device context
HGLRC ghrc = NULL; //Handle to OpenGL rendering context
bool GBActiveWindow = false; //Flag for active window
bool bFullScreen = false; //Flag for full screen
DWORD dwStyle; //Variable to hold window style
HWND gHwnd = NULL; //Handle to window
static GLfloat angle = 0.0f; //Angle for rotation
bool anticlockwise_totation = true; //Flag for rotation
bool clockwise_rotation = false;	//Flag for rotation


//Shader Objects
GLuint gVertexShaderObject;
GLuint gFragmentShaderObject;
GLuint gShaderProgramObject;


//window placement structure
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

//global function declarations for fullscreen function 
void ToggleFullScreen(void);

//global function declarations -> window procedure function 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


//every desktop application must have WinMain() function 

/*
int WINAPI WinMain(
   HINSTANCE hInstance       => handle to the current instance of the applocation,
   HINSTANCE hPrevInstance   => handle to the previous instance of the application ,
   LPSTR lpszCmdLine         => the command line for the application excluding the program name ,
   int  iCmdShow             =>  Controls how the window is to be shown when program is launched.
);
*/

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpszCmdLine,
    _In_ int iCmdShow
) {
    // function declarations
    int initialize(void);
    void display(void);
	 
    // variable declarations
    bool bDone = false; // Flag to check if the program should exit
	int iRet = 0; // variable to store return value
	WNDCLASSEX wndClass; // window class structure
	HWND hwnd; // handle to window
	MSG msg; // message structure
    TCHAR szAppName[] = TEXT("PPL-BlueWindow-Template-Shader"); 

    if (fopen_s(&gpFile, "Log.txt", "w") != 0) {
        MessageBox(NULL, TEXT("Log File Cannot Be Created"), TEXT("Error"), MB_OK);
    } else {
        fprintf(gpFile, "Log File Created \n");
    }

    // The string that appears in the application's title bar use this in createWindow 
    static TCHAR szTitle[] = TEXT("Yashwant's Shader Window ");

    // to store some basic information about we use window class 

    // initialization of WNDCLASSEX 
    wndClass.cbSize = sizeof(WNDCLASSEX);
    wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraws the entire window if a movement or size adjustment changes the height or width of the client area.
    wndClass.cbClsExtra = 0; // The number of extra bytes to allocate following the window-class structure.
    wndClass.cbWndExtra = 0; // The number of extra bytes to allocate following the window instance
    wndClass.lpfnWndProc = WndProc; // A pointer to the window procedure function which handle events we have defined that function and registering here 
    wndClass.hInstance = hInstance; // a handle to the instance that contains the window procedure for the class.
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); // A handle to the class icon
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // A handle to the class cursor 
    wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); // A handle to the small icon associated with this window
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // A handle to the class background brush change this if you want diff background i.e BLACK_BRUSH
    wndClass.lpszClassName = szAppName; // class name 
    wndClass.lpszMenuName = NULL; // give menu name that you created using .r c file 

    // register above class with windows so that it knows about your windows and how to send messages to it 
    RegisterClassEx(&wndClass);

    // Create Window using CreateWindow 
    hwnd = CreateWindow(szAppName,
        szTitle, // title of the window
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, // Window style 
        100, // x start position we are setting 100 window start from x 100
        100, // Y start position we are setting 100 
        WIN_WIDTH, // width of the window
        WIN_HEIGHT, // height of the window
        NULL, // we are setting null means our parent is OS. otherwise we can set parent window handle 
        NULL, // we are setting null means there is no menu otherwise need to set menu handle 
        hInstance, // Handle to the instance of the module to be associated with the window.
        NULL
    );

    // Store window handle in our global variable
    gHwnd = hwnd;
    iRet = initialize();

    // checking errors 
    if (iRet == -1) {
        fprintf(gpFile, "Choose Pixel Format Failed \n");
        DestroyWindow(hwnd);
    } else if (iRet == -2) {
        fprintf(gpFile, "Set Pixel Format Failed \n");
        DestroyWindow(hwnd);
    } else if (iRet == -3) {
        fprintf(gpFile, "wglCreateContextFailed \n");
        DestroyWindow(hwnd);
    } else if (iRet == -4) {
        fprintf(gpFile, "wglMakeCurrentFailed \n");
        DestroyWindow(hwnd);
    } else {
        fprintf(gpFile, "initialize is successful :) \n");
    }

    // our windows has been created by CreateWindow now we need to tell windows that to make it visible.
    ShowWindow(hwnd, iCmdShow);
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // to handle messages we add message loop to listen messages that windows send
    // When the application receives a message, this loop dispatches it to WndProc function to be handled.
    while (bDone == false) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                bDone = true;
            } else {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        } else {
            if (GBActiveWindow == true) {
                // Add any additional processing here
            }
            display();
        }
    }

    return ((int)msg.wParam);
}

/*
  WndProc function is needed to handle the messages application receives
  from windows when events occur. we register this function when we created window class above in winmain function.
  You can give whatever name you like. WndProc is standard.
  */
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	//code 

	void resize(int, int);
	void uninitialize(void);

	switch (iMsg) {

		GBActiveWindow = true; // Set active window flag
		break;
	case WM_KILLFOCUS:
		GBActiveWindow = false; // Clear active window flag
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam)); // Resize Viewport
		break;
	case WM_ERASEBKGND:
		return(0); // Return 0 to prevent flickering
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd); // Destroy Window
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd); // Destroy Window on Escape Key
			break;

		case 0x46: // 'f' or 'F' key for full screen
			if (bFullScreen == false)
			{
				ToggleFullScreen();
			}
			else
			{
				ToggleFullScreen();
			}
			break;
		}
		break;

	case WM_DESTROY:
		uninitialize();
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


int initialize() {
	void resize(int, int);
	void uninitialize();

	//variable declarations 
	PIXELFORMATDESCRIPTOR pfd; // pixel format descriptor
	int iPixelFormatIndex; // index of pixel format
	GLenum result; // to store the result of glewinit

	//code 
	//initilize pfd structure 
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 32;
	ghdc = GetDC(gHwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd); // choose pixel format
	if (iPixelFormatIndex == 0)
	{
		return (-1); // Error Choosing Pixel Format
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE)
	{
		return(-2); // Error Setting Pixel Format
	}

	ghrc = wglCreateContext(ghdc); // Create OpenGL Rendering Context

	if (ghrc == NULL) {
		return(-3); // Error Creating Rendering Context
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4); // Error Making Current Rendering Context
	}

	result = glewInit(); // Initialize GLEW Library


	if (result != GLEW_OK) {
		fprintf(gpFile, "glew init failed");
		uninitialize();
		DestroyWindow(gHwnd);
	}

	//create Vertex Shader
	gVertexShaderObject = glCreateShader(GL_VERTEX_SHADER);

	//vertex shader source code
	const GLchar* vertexShaderSourceCode =
		"#version 140 \n"  
		"void main(void) \n" 
		"{ \n" 
		"}";

	//Specify above source code to vertex shader object
	glShaderSource(gVertexShaderObject, 1, (const GLchar**)&vertexShaderSourceCode, NULL);

	//compile the vertex shader
	glCompileShader(gVertexShaderObject);

	//Error checking for compilation vertex shader
	GLint iInfoLogLength = 0;
	GLint iShaderCompiledStatus = 0;
	GLchar* szInfoLog = NULL;
	glGetShaderiv(gVertexShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);

	if (iShaderCompiledStatus == GL_FALSE) {
		glGetShaderiv(gVertexShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);//To get error length
		if (iInfoLogLength > 0) {
			szInfoLog = (GLchar*)malloc(iInfoLogLength); // Dynamic memory allocation
			if (szInfoLog != NULL) {
				GLsizei written; //stores the number of characters written in the string
				glGetShaderInfoLog(gVertexShaderObject, iInfoLogLength, &written, szInfoLog); // To get error message in szInfoLog
				fprintf(gpFile, "Vertex Shader Compilation Log : %s \n", szInfoLog); // Print error message in log file
				free(szInfoLog); // Free the memory
				uninitialize(); // Exit the application
				DestroyWindow(gHwnd); // Close the window
				exit(0); // Exit the application
			}
		}
	}

	//create fragment shader
	gFragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);

	//Fragment shader source code
	const GLchar *fragmentShaderSourceCode = 
		"#version 140\n"
		"void main(void)\n"
		"{\n"
		"}";
	
	//Specify above source code to fragment shader object
	glShaderSource(gFragmentShaderObject, 1, (const GLchar**)&fragmentShaderSourceCode, NULL);


	//compile the fragment shader
	glCompileShader(gFragmentShaderObject);

	//Error checking for compilation fragment shader
	iShaderCompiledStatus = 0;
	iInfoLogLength = 0;
	szInfoLog = NULL;

	glGetShaderiv(gFragmentShaderObject, GL_COMPILE_STATUS, &iShaderCompiledStatus);


	if (iShaderCompiledStatus == GL_FALSE) 
	{
		glGetShaderiv(gFragmentShaderObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0)
		{
			szInfoLog = (GLchar*)malloc(iInfoLogLength);

			if (szInfoLog != NULL)
			{			
			GLsizei written;
			glGetShaderInfoLog(gFragmentShaderObject,iInfoLogLength,&written, szInfoLog);
			fwprintf(gpFile, TEXT("Fragment Shader Compilation Log : %hs \n"), szInfoLog);
			free(szInfoLog);
			uninitialize();
			DestroyWindow(gHwnd);
			exit(0); // Exit the application
			}
		}
	}


	//Create Shader Program Object
	gShaderProgramObject = glCreateProgram();

	//attach vertex shader to shader program
	glAttachShader(gShaderProgramObject, gVertexShaderObject);

	//attach fragment shader to shader program
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);


	//Link the shader program
	glLinkProgram(gShaderProgramObject);


	//Error checking for shader program 
	GLint iProgramLinkStatus = 0;
	iInfoLogLength = 0;
	szInfoLog = NULL;

	glGetProgramiv(gShaderProgramObject, GL_LINK_STATUS, &iProgramLinkStatus);

	if (iProgramLinkStatus == GL_FALSE) {
		glGetProgramiv(gShaderProgramObject, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		if (iInfoLogLength > 0) {

			szInfoLog = (GLchar*)malloc(iInfoLogLength); // Dynamic memory allocation for storing the error message
			if (szInfoLog != NULL) {
			
				GLsizei written;
				glGetProgramInfoLog(gShaderProgramObject, iInfoLogLength, &written, szInfoLog);
				fwprintf(gpFile, TEXT("Shader Program Link Log : %hs \n"), szInfoLog);

				free(szInfoLog);
				uninitialize();
				DestroyWindow(gHwnd);
				exit(0);
			}
		}
	}

	glShadeModel(GL_SMOOTH); // Set the shading model

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f); // Set clear color to blue
	glClearDepth(1.0f); // Set the depth value to 1.0f
	glEnable(GL_DEPTH_TEST); //		Enable the depth test
	glDepthFunc(GL_EQUAL); //	Set the depth function
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Set the perspective correction hint
	resize(WIN_WIDTH, WIN_HEIGHT); //	Resize the window / viewport

	return (0);
}

void resize(int width, int height)
{
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void display(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer and depth buffer
	glUseProgram(gShaderProgramObject); // Use the shader program object
	glUseProgram(0); // stop using the shader program object
	SwapBuffers(ghdc);
}

void uninitialize(void)
{
	if (bFullScreen == true) {
		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wpPrev);
		SetWindowPos(gHwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		bFullScreen = false;
	}

	if (wglGetCurrentContext() == ghrc) {
		wglMakeCurrent(NULL, NULL);
	}

	if (ghrc) {
		wglDeleteContext(ghrc);
		ghrc = NULL;
	}

	if (ghdc) {
		ReleaseDC(gHwnd, ghdc);
		ghdc = NULL;
	}

	if (gpFile) {
		fprintf(gpFile, "Log File Exit \n");
		fclose(gpFile);
		gpFile = NULL;
	}

	glUseProgram(gShaderProgramObject);
	glDetachShader(gShaderProgramObject, gVertexShaderObject);
	glAttachShader(gShaderProgramObject, gFragmentShaderObject);

	glDeleteShader(gFragmentShaderObject);
	gFragmentShaderObject = 0;

	glDeleteShader(gVertexShaderObject);
	gVertexShaderObject = 0;


	glDeleteProgram(gShaderProgramObject); //Delete shader program object 
	gShaderProgramObject = 0;

	glUseProgram(0); // Use the shader program object

}

void ToggleFullScreen(void) {
	MONITORINFO mi;
	if (bFullScreen == false)
	{
		dwStyle = GetWindowLong(gHwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			mi = { sizeof(MONITORINFO) };
			if (GetWindowPlacement(gHwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(gHwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(gHwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(gHwnd,
					HWND_TOP,
					mi.rcMonitor.left,
					mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}
		ShowCursor(FALSE);
		bFullScreen = true;
	}
	else
	{
		SetWindowLong(gHwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(gHwnd, &wpPrev);
		SetWindowPos(gHwnd,
			HWND_TOP,
			0,
			0,
			0,
			0,
			SWP_NOZORDER | SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER);
		ShowCursor(TRUE);
		bFullScreen = false;
	}
}


