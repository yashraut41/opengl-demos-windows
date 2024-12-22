

Step 1: \
Install Visual Studio 2022 make sure you have Desktop application with C++ workloads selected  
![Logo](https://learn.microsoft.com/en-us/cpp/build/media/desktop-development-with-cpp.png)

Step 2: \
Open Visual Studio and select create nw project option 

![Step 2](https://github.com/user-attachments/assets/8f3d7930-b6fe-4043-9943-d9b7fb4abf2b)

Step 3: \
At the top of the dialog, set Language to C++, set Platform to Windows, and set Project type to Desktop and choose Windows Desktop Wizard then choose Next

![Step 3](https://github.com/user-attachments/assets/9ecfcb5f-2cb8-4a57-8fb4-535e5f824d32)

Step 4: \
Enter a name for the project i.e. BlueWindow and Choose the Create button to create the project.

Step 5: \
The Windows Desktop Project dialog now appears. In the Application type dropdown, make sure you select Desktop application (.exe). Then, under Additional options, select Empty project. Choose OK to create the project.

![Step 5](https://github.com/user-attachments/assets/d5c9479f-79b2-4605-a373-ff36313f2104)

Step 6: \
Now in IDE Solution Explorer, right-click the DesktopApp project, choose Add, and then choose New Item.

![Step 6](https://learn.microsoft.com/en-us/cpp/build/media/desktop-app-project-add-new-item-153.gif)

Step 7: \
In the Add New Item dialog box, select C++ File (.cpp). In the Name box, type a name for the file, for example, HelloWindowsDesktop.cpp. Choose Add.

![Step 7](https://learn.microsoft.com/en-us/cpp/build/media/desktop-app-add-cpp-file-153.png)

Step 8: \
Now add code given in the repo folder [link](https://github.com/yashraut41/opengl-demos-windows/blob/main/BlueWindow/BlueWindow.cpp) 

Step 9: \ 
Need to add glew library for that Get Binaries ( .Dll And .Lib ) And Headers ( .h ) From [here](https://glew.sourceforge.net/index.html)

- Extract The Zip File To Its Default Directory : glew-<version number>-win32

- Rename It To Just 'glew' And Then Copy This whole Directory As It Is To C:\

- So That Now You Have => C:\glew like this ![image](https://github.com/user-attachments/assets/66e36c83-e143-4db5-9a7f-e4c5dc4b4da4)

Step 10: \
In A Win32 Project > In Visual Studio > Solution Explorer > Right Click On Project Name > 
  Properties > Left Pane > Configuration Properties : then 
  A => VC++ Directories > Right Pane > Include Directories > 
     Go To End Of Line, Type Semicolon If Already Not There, Type => C:\glew\include

  B => VC++ Directories > Right Pane > Library Directories > 
      Go To End Of Line, Type Semicolon If Already Not There, And Then Type => C:\glew\lib\Release\x64
   ![Screen Recording 2024-12-17 222350](https://github.com/user-attachments/assets/5c812b9c-377e-41d4-ac2e-af886d58afd5)


Step 11: \ 
Build the code and click debug -> start debugging you will see blue window like this 

![Step 11](https://raw.githubusercontent.com/yashraut41/opengl-demos-windows/refs/heads/main/BlueWindow/image.png)

