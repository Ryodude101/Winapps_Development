/*
    Learning Windows API
    Ryan Colon
    1.7.2022
*/

//Necessary to define for the way this program is written. Need to look into.
#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

/*
    Should be created even if not explicitely used. It is how the OS communicates with the program. This is the default provided by the tutorial but I believe you can modify 
    what happens when the program recieves a message and thusly handle sys calls gracefully.
    LRESULT is an integer value the application returns to windows. The meaning is dependant on the message received.
    This function is blocking while it runs so plan accordingly.
*/
LRESULT CALLBACK WindowProc(
    HWND hwnd, //window handle 
    UINT uMsg, //message code
    WPARAM wParam, // Both wParam and lParam are extra message information. What it is depends on the message. Both are integers (width determined by system).
    LPARAM lParam  //Lookup message code on MSDN and cast data to correct type
    ){
    

    //Message code is an integer, so most easily handled as a switch statement.
    switch(uMsg){
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT: //Always called at least once, at start to paint the whole space. Subsequent calls might only need a specific area updated.
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            //I believe this is how you color the interior of the window. Further study required.
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1)); //rcPaint member contains the update area.

            EndPaint(hwnd, &ps); //Clears the update regions and signals the op system painting is done 
        }
        return 0;

        case WM_CLOSE:
            DestroyWindow(hwnd);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Returns the default response for the message to the system. Put at the end of switch statement in case you dont handle a
                                                      // specific sys message. Also useful for easily handling a message.
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow){
    /* 
        Every window must be associated with a window block
        Not the same as a C++ class, declared and utilized in the operating system at run time
    */ 
    const wchar_t CLASS_NAME[] = L"Sample Window Class";    
                                                            
    WNDCLASS wc = { }; //Instantiate the window class                            

    //These 3 members must be initialized                                                        
    wc.lpfnWndProc = WindowProc; //Pointer to the application function that defines the behavior of this window, called by DispatchWindow if the window is the target                            
    wc.hInstance = hInstance; // Handle to the application instance (believe this is handed to us from the op system at the beginning of winmain)                             
    wc.lpszClassName = CLASS_NAME; //String that identifies the window class, name only needs to be unique to the process
    //Standard windows controls have classes. If used, pick class names that do not conflict with control class names

    RegisterClass(&wc); //Register the class with the operating system

    //Create the instance of the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"This is so cool!",                // Window text
        WS_OVERLAPPEDWINDOW,            // Window style (controls whats included, min/max/close buttons, a title bar, outlines, etc.). A group of flags.

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data (arbitrary)
    );

    if (hwnd == NULL) //Couldn't create the window
    {
        return 0;
    }                          

    ShowWindow(hwnd, nCmdShow);

    //Communicate with the operating system
    MSG msg = { };
    //For every thread that creates a window, the operating system creates a queue to contain messages
    while (GetMessage(&msg, NULL, 0, 0) > 0) //Removes message from head of the queue. Will block if empty. For background processes, create new thread.
    {                                        //Fills the first parameter (a message structure) with the message information.
        TranslateMessage(&msg); //Related to keyboard input, translates keystrokes (keyup, keydown) into characters. Must be called before DispatchMessage().
        DispatchMessage(&msg); // Tell the op system to call the WinProc of the target window of the message
    }
    //When you want to exit the application and break out of the message loop, call the PostQuitMessage(0) function.

    return 0;
}

