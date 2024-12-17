#include<Windows.h>
#include<gl/glew.h>
#include<gl/GL.h>
#include<stdio.h>

#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"opengl32.lib")


#define WIN_WIDTH 800
#define WIN_HEIGHT 600

FILE* gpFile;
HDC ghdc = NULL;
HGLRC ghrc = NULL;
bool GBActiveWindow = false;
bool bFullScreen = false;
DWORD dwStyle;
HWND gHwnd = NULL;
static GLfloat angle = 0.0f;
bool anticlockwise_totation = true;
bool clockwise_rotation = false;

WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {


	//function declarations
	
	int initialize(void);
	void display(void);


	//variable declarations
	bool bDone = false;
	int iRet = 0;
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PPL-BlueWindow-Template");

	if (fopen_s(&gpFile, "Log.txt", "w") != 0) {
		MessageBox(NULL, TEXT("Log File Cannot Be Created"), TEXT("Error"), MB_OK);
	}
	else {
		fprintf(gpFile, "Log File Created \n");
	}

	/// The string that appears in the application's title bar use this in createWindow 
	static TCHAR szTitle[] = TEXT("Yashwant's Blue Window");

	//to store some basic information about we use window class 

	//initialization of WNDCLASSEX 
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Redraws the entire window if a movement or size adjustment changes the height or width of the client area.
	wndClass.cbClsExtra = 0; //The number of extra bytes to allocate following the window-class structure.
	wndClass.cbWndExtra = 0; // The number of extra bytes to allocate following the window instance
	wndClass.lpfnWndProc = WndProc; //A pointer to the window procedure function which handle events we have defined that function and registering here 
	wndClass.hInstance = hInstance; // a handle to the instance that contains the window procedure for the class.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //A handle to the class icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // A handle to the class cursor 
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//A handle to the small icon associated with this window
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //A handle to the class background brush change this if you want diff background i.e BLACK_BRUSH
	wndClass.lpszClassName = szAppName; //class name 
	wndClass.lpszMenuName = NULL; //give menu name that you created using .r c file 
	

	//register above class with windows so that it knows about your windows and how to send messages to it 
	RegisterClassEx(&wndClass);

	//Create Window using CreateWindow 
	hwnd = CreateWindow(szAppName,
		szTitle, //title of the window
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE, // Window style 
		100, // x start position we are setting 100 window start from x 100
		100, // Y start position we are setting 100 
		WIN_WIDTH,//  widhth of the window
		WIN_HEIGHT, // height of the window
		NULL, //we are settng null means our parent is OS. otherwise we can set parent window handle 
		NULL, //we are setting null means there is no menu otherwise need to set menu handle 
		hInstance, //Handle to the instance of the module to be associated with the window.
		NULL
	);

	// Store window handle in our global variable
	gHwnd = hwnd;

	iRet = initialize();
	//checking errors 
	if (iRet == -1) {
		fprintf(gpFile, "Choose Pixel Format Failed \n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -2)
	{
		fprintf(gpFile, "Set Pixel Format Failed \n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -3)
	{
		fprintf(gpFile, "wglCreateContextFailed \n");
		DestroyWindow(hwnd);
	}
	else if (iRet == -4)
	{
		fprintf(gpFile, "wglMakeCurrentFailed \n");
		DestroyWindow(hwnd);
	}
	else
	{
		fprintf(gpFile, "initialize is successfull :) \n");
		
	}

	/*
	our windows has been created by CreateWindow now we need to tell windows that to make it visible.
	*/
	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);
	

	/*
	to handle messages we add message loop to listen messages that windows send
	When the application receives a message, this loop dispatches it to WndProc function to be handled.
	*/
	 
	while (bDone == false)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
		
		   if (msg.message == WM_QUIT) 
		    {
				bDone = true;
	    	}		
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			if (GBActiveWindow == true) {

			  }
			display();
		}
		
	}

	return((int)msg.wParam);
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

	GBActiveWindow = true;
	break;
	case WM_KILLFOCUS:
		GBActiveWindow = false;
		break;
	case WM_SIZE:
		resize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_ERASEBKGND:
		return(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		case 0x46:
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
	PIXELFORMATDESCRIPTOR pfd;
	int iPixelFormatIndex;
	GLenum result;

	//code 
	//initilize pfd structure 
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 8;
	pfd.cRedBits = 8;
	pfd.cGreenBits = 8;
	pfd.cBlueBits = 8;
	pfd.cAlphaBits = 8;
	pfd.cDepthBits = 8;
	ghdc = GetDC(gHwnd);

	iPixelFormatIndex = ChoosePixelFormat(ghdc, &pfd);
	if (iPixelFormatIndex == 0) 
	{
		return (-1);
	}

	if (SetPixelFormat(ghdc, iPixelFormatIndex, &pfd) == FALSE) 
	{
		return(-2);
	}

	ghrc = wglCreateContext(ghdc);

	if (ghrc == NULL) {
		return(-3);
	}

	if (wglMakeCurrent(ghdc, ghrc) == FALSE)
	{
		return(-4);
	}

	result = glewInit();

	if (result != GLEW_OK) {
		fprintf(gpFile, "glew init failed");
		uninitialize();
		DestroyWindow(gHwnd);
	}

	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	resize(WIN_WIDTH, WIN_HEIGHT);

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


