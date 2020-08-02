#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>

#include"PhysStructs.h"
#include"PhysFuncs.h"
#include"GameTimer.h"
#include"Collision.h"

#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

HDC g_hdc;//global handle to device context
int g_width, g_height;//width and height of the window

std::vector<PhysObj> physObjs;


void setupPixelFormat(HDC hdc) {
	int nPixelFormat;

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), //size of structure
		1,	//version (always 1)
		PFD_DRAW_TO_WINDOW | //support allowing opengl to draw to a window
		PFD_SUPPORT_OPENGL | //support opengl
		PFD_DOUBLEBUFFER, //double buffering
		PFD_TYPE_RGBA, //iPixelType rgba rather than colorindex
		32,	//attempt 32 bit color
		0,0,0,0,0,0, //ignore color bits
		0,	//no alpha buffer
		0,	//ignore shift bit
		0,	//no accumulation buffer
		0,0,0,0,	//ignore accumulation bits
		16,	//16 bit z-buffer size
		0,	//no stencil buffer
		0,	//no auziliary buffer
		PFD_MAIN_PLANE,	//main drawing plane
		0,	//reserved
		0,0,0 };	//layer masks ignored

	//choose the best matching pixel format and return the index number
	nPixelFormat = ChoosePixelFormat(hdc, &pfd);

	//set pixel format to device context
	SetPixelFormat(hdc, nPixelFormat, &pfd);
}

//create wndProc
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	static HDC hDC;//handle to device context
	static HGLRC hRC;//Handle to rendering context

	switch (message) {
	case WM_CREATE:
		hDC = GetDC(hwnd);//get current device context
		g_hdc = hDC;
		setupPixelFormat(hDC);//setup pixel device context within function

		hRC = wglCreateContext(hDC);//create rendering context;
		wglMakeCurrent(hDC, hRC);//make it the current rendering context for gl to draw to;
		//make current NULL before deleting rendering context, and before posting the quit message		

		return 0;
	case WM_CLOSE:
		wglMakeCurrent(hDC, NULL);//clear the current context so that it is not accidently drawn to after deleting
		wglDeleteContext(hRC);

		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		return 0;
	case WM_SIZE:
		g_width = LOWORD(lParam);
		g_height = HIWORD(lParam);

		if (g_height == 0)
			g_height = 1;

		//reset viewport to new dimensions
		glViewport(0, 0, g_width, g_height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();//reset projection matrix
		//load entity puts the matrix on to the current matrix stack

		gluOrtho2D(0, g_width, 0, g_height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

	default:
		break;
	}

	//pass all unhandled messages to the os with DefWindowProc
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void drawObj(PhysObj obj) {
	glTranslatef(obj.position.x, obj.position.y, 0.0f);
	glRotatef(obj.rotation, 0.0f, 0.0f, 1.0f);

	glBegin(GL_LINES);
	for (int i = 0; i < obj.verts.size() - 2; i++) {
		glVertex2f(obj.verts[i], obj.verts[i + 1]);
		glVertex2f(obj.verts[i + 2], obj.verts[i + 3]);
		i += 1;
	}
	glVertex2f(obj.verts[obj.verts.size() - 2], obj.verts[obj.verts.size() - 1]);
	glVertex2f(obj.verts[0], obj.verts[1]);
	glEnd();

	glLoadIdentity();
}

//draw object by its manually manipulated world vectors
void drawObjWorldVects(PhysObj obj) {
	glBegin(GL_LINES);
	for (int i = 0; i < obj.worldVerts.size(); i += 2) {
		glVertex2f(obj.worldVerts[i], obj.worldVerts[(i + 1) % obj.worldVerts.size()]);
		glVertex2f(obj.worldVerts[(i + 2) % obj.worldVerts.size()], obj.worldVerts[(i + 3) % obj.worldVerts.size()]);
	}
	glEnd();

}



//Create WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nShowCmd) {
	//MessageBox(NULL, "Hello World!", "Window title?", NULL);


	PhysObj square;
	square.rotation = 0.0f;
	square.position = Vec2d{ 60,260 };
	square.verts.push_back(-20);
	square.verts.push_back(-20);

	square.verts.push_back(20);
	square.verts.push_back(-20);

	square.verts.push_back(20);
	square.verts.push_back(20);

	square.verts.push_back(-20);
	square.verts.push_back(20);

	setObjCenter(&square);
	//square.center = Vec2d{ 0,0 };

	physObjs.push_back(square);

	PhysObj rect;
	//rect.rotation = 315.0f;
	rect.rotation = 340.0f;
	rect.position = Vec2d{ 100,290 };
	rect.verts.push_back(-40);
	rect.verts.push_back(-2);

	rect.verts.push_back(40);
	rect.verts.push_back(-2);

	rect.verts.push_back(40);
	rect.verts.push_back(2);

	rect.verts.push_back(-40);
	rect.verts.push_back(2);

	setObjCenter(&rect);
	//rect.center = Vec2d{ 0,0 };

	physObjs.push_back(rect);


	MSG msg;

	//class setup
	//class registration
	WNDCLASSEX windowClass;
	windowClass.cbSize = sizeof(WNDCLASSEX);//set size to that of the window class structure
	windowClass.style = CS_HREDRAW | CS_VREDRAW; //redraw on size
	windowClass.lpfnWndProc = (WNDPROC)WndProc; //pointer to the windows process created earlier
	windowClass.cbClsExtra = 0; //how many extra bytes to allocate after the window class structure
	windowClass.cbWndExtra = 0; //how many extra bytes to allocate after the window instance
	windowClass.hInstance = hInstance; //application instance handle
	windowClass.hIcon = NULL;
	windowClass.hCursor = LoadCursor(hInstance, IDC_ARROW);//load default arrow cursor
	windowClass.hbrBackground = NULL;// no background brush
	windowClass.lpszMenuName = NULL;//no menu
	windowClass.lpszClassName = "MyClass";//window class name
	windowClass.hIconSm = LoadIcon(hInstance, IDI_WINLOGO);

	RegisterClassEx(&windowClass);//register the window class



	//window creation
	HWND hwnd = CreateWindowEx(NULL,
		"MyClass",//window class name
		"2D GL Physics",//window title
		WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,//style
		400, 400,//x,y coordinates
		800, 600,//width and height
		NULL,//handle to parent (no parent)
		NULL,//handle to menu(no menu)
		hInstance,//application instance handle
		NULL);//no extra creation parameters

	ShowWindow(hwnd, nShowCmd);



	double timeCounter = 0.0f;
	GameTimer gt;
	gt.init();

	//message loop with event handler
	bool done = false;
	float angle = 0.0;
	while (!done) {
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);//check for pending message
		if (msg.message == WM_QUIT) {
			done = true;
		}
		else {

			double deltaTime = gt.getElapsedTimeInSeconds();
			timeCounter += deltaTime;

			//clear screen and depth buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();

			angle += 50.050f*deltaTime;//deltaTime;
			if (angle > 360.0f)angle = 0.0f;

			timeCounter = 0.0f;

			physObjs[1].rotation = angle;

			//Update positions and rotations
			updateWorldVerts(&physObjs[0]);
			updateWorldVerts(&physObjs[1]);

			//Check for collision, if there is, draw in red
			if (Collision::SATCollision(physObjs[0], physObjs[1])) {
				glColor3f(1.0f, 0.0f, 0.0f);
			}
			else {
				glColor3f(1.0f, 1.0f, 1.0f);
			}

			for (auto ob : physObjs) {
				drawObjWorldVects(ob);
				//drawObj(ob);
			}





			SwapBuffers(g_hdc);


			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
