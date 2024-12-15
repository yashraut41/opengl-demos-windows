#include<Windows.h>
 
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

	//variable declarations
	WNDCLASSEX wndClass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

	/// The string that appears in the application's title bar use this in createWindow 
	static TCHAR szTitle[] = TEXT("Yashwant's Window");

	//to store some basic information about we use window class 

	//initialization of WNDCLASSEX 
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW; //Redraws the entire window if a movement or size adjustment changes the height or width of the client area.
	wndClass.cbClsExtra = 0; //The number of extra bytes to allocate following the window-class structure.
	wndClass.cbWndExtra = 0; // The number of extra bytes to allocate following the window instance
	wndClass.lpfnWndProc = WndProc; //A pointer to the window procedure function which handle events we have defined that function and registering here 
	wndClass.hInstance = hInstance; // a handle to the instance that contains the window procedure for the class.
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION); //A handle to the class icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW); // A handle to the class cursor 
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); //A handle to the class background brush change this if you want diff background i.e BLACK_BRUSH
	wndClass.lpszClassName = szAppName; //class name 
	wndClass.lpszMenuName = NULL; //give menu name that you created using .r c file 
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);//A handle to the small icon associated with this window

	//register above class with windows so that it knows about your windows and how to send messages to it 
	RegisterClassEx(&wndClass);
 
	//Create Window using CreateWindow 
	hwnd = CreateWindow(szAppName,
		szTitle, //title of the window
		WS_OVERLAPPEDWINDOW, // Window style 
		CW_USEDEFAULT, // x start position we are setting default if we set 0 window will be on left
		CW_USEDEFAULT, // here also we are setting y defualt position if we set 0 along with x 0 window will be on left top
		CW_USEDEFAULT,//  widhth of the window we set it to default  can be set 500
		CW_USEDEFAULT, // height of the window we set it to defailt can be set 100
		NULL, //we are settng null means our parent is OS. otherwise we can set parent window handle 
		NULL, //we are setting null means there is no menu otherwise need to set menu handle 
		hInstance, //Handle to the instance of the module to be associated with the window.
		NULL
	);

	/*
	our windows has been created by CreateWindow now we need to tell windows that to make it visible.    
	*/	 
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	/*
	to handle messages we add message loop to listen messages that windows send
    When the application receives a message, this loop dispatches it to WndProc function to be handled.
	*/
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
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
	switch (iMsg) {

	 
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}