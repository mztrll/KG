#include <windows.h>
#include <gl/gl.h>
#include <math.h>

float vert[] = {1,1,0, 1,-1,0, -1,-1,0, -1,1,0}; //вершины
float xAlfa = 20;
float zAlfa = 0;
POINTFLOAT pos = {0, 0};
float pyramid[] = {4,4,2, 5,5,0, 5,3,0, 3,3,0, 3,5,0, 5,5,0};

int n = 10;

void Init()
{
    glEnable(GL_BLEND); //включение смешивания цветов
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //настройка смешивания цветов (цвет источника, цвет приемщика)
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
}

void ShowWorld()
{
    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vert);
        //Задание 3
        for (int i = -n; i < n; i++)
        {
            for (int j = -n; j < n; j++)
            {
                glPushMatrix();
                    if ((i+j) % 2 == 0) glColor3f(0.8, 0.8, 0.8);
                    else glColor3f(0.2, 0.2, 0.2);
                    glTranslatef(i*2, j*2, 0);
                    glDrawArrays(GL_TRIANGLE_FAN, 0 ,4);
                glPopMatrix();
            }
        }
        glVertexPointer(3, GL_FLOAT, 0, &pyramid);
        glColor3f(1,0,0);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

        //Задание 1
        glLineWidth(5);
        glBegin(GL_LINES);

            glColor3f(1,0,0);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 1, 1);

            glColor3f(0,1,0);
            glVertex3f(0, 0, 1);
            glVertex3f(1, 0, 1);

            glColor3f(0,0,1);
            glVertex3f(0, 0, 1);
            glVertex3f(0, 0, 2);

        glEnd();

    glDisableClientState(GL_VERTEX_ARRAY);
}

void MoveCamera()
{
    if (GetKeyState(VK_UP) < 0) xAlfa = ++xAlfa > 180 ? 180 : xAlfa;
    if (GetKeyState(VK_DOWN) < 0) xAlfa = --xAlfa < 0 ? 0 : xAlfa;
    if (GetKeyState(VK_LEFT) < 0) zAlfa += 1;
    if (GetKeyState(VK_RIGHT) < 0) zAlfa -= 1;

    float angle = -zAlfa / 180 * M_PI;
    float speed = 0;
    if (GetKeyState('W') < 0) speed = 0.1;
    if (GetKeyState('S') < 0) speed = -0.1;
    if (GetKeyState('A') < 0) { speed = 0.1; angle -= M_PI*0.5; }
    if (GetKeyState('D') < 0) { speed = 0.1; angle += M_PI*0.5; }
    if (speed != 0)
    {
        pos.x += sin(angle) * speed;
        pos.y += cos(angle) * speed;
    }

    glRotatef(-xAlfa, 1, 0, 0);
    glRotatef(-zAlfa, 0, 0, 1);
    glTranslatef(-pos.x, -pos.y, -3);
}

float vertex[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1}; //вершины
float normal[] = {-1,-1,1, 1,-1,1, 1,1,1, -1,1,1}; //векторы нормали

void Draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertex);
        glNormalPointer(GL_FLOAT, 0, normal);
        glDrawArrays(GL_POLYGON, 0, 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void DrawCube()
{
    glPushMatrix();
        glScalef(1, 1, 1);  //размерность куба
        glTranslatef(-6, -4, 1);    //расположение куба в пространстве
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 1,0,0);
        Draw();
        glRotatef(90, 0,1,0);
        Draw();
        glRotatef(180, 0,1,0);
        Draw();
    glPopMatrix();
}

void DrawPrism(int n)
{
     const float pi = acos(-1);
    const int vn = 3*n; // количество вершин

    float vertieces[3*vn]; // Это же и нормали.

    for (int i = 0; i < n; i++) {
        float phi = 2 * pi * (float)i / (float)n;
        float c = cos(phi);
        float s = sin(phi);

        // Вершина в верхнем основании
        vertieces[9*i + 0] = c;
        vertieces[9*i + 1] = s;
        vertieces[9*i + 2] = 1.0;

        // cthtlbyf
        vertieces[9*i + 3] = 0.7*c;
        vertieces[9*i + 4] = 0.7*s;
        vertieces[9*i + 5] = 0.0;

        // Вершина в нижнем основании
        vertieces[9*i + 6] = c;
        vertieces[9*i + 7] = s;
        vertieces[9*i + 8] = -1.0;

    }

    glPushMatrix();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertieces);
    glNormalPointer(GL_FLOAT, 0, vertieces);

    // Боковые стороны
    GLuint side_index[4];
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 4; k++) {
            side_index[k] = (3*(i + k/2) + k%2) % vn;
        }

        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, &side_index);
    }

    for (int i = 0; i < n; i++) {
        for (int k = 0; k < 4; k++) {
            side_index[k] = (3*(i + k/2) + k%2 + 1) % vn;
        }

        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, &side_index);
    }

    // Верхнее основание
    GLuint base_index[n];
    for (int k = 0; k < n; k++) {
        base_index[k] = 3*k;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);

    // Нижнее основание
    for (int k = 0; k < n; k++) {
        base_index[k] = 3*k + 2;
    }
    glDrawElements(GL_POLYGON, n, GL_UNSIGNED_INT, &base_index);


    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glPopMatrix();
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC*, HGLRC*);
void DisableOpenGL(HWND, HDC, HGLRC);


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    WNDCLASSEX wcex;
    HWND hwnd;
    HDC hDC;
    HGLRC hRC;
    MSG msg;
    BOOL bQuit = FALSE;
    float theta = 0.0f;

    /* register window class */
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "GLSample";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);;


    if (!RegisterClassEx(&wcex))
        return 0;

    /* create main window */
    hwnd = CreateWindowEx(0,
                          "GLSample",
                          "OpenGL Sample",
                          WS_OVERLAPPEDWINDOW,
                          CW_USEDEFAULT,
                          CW_USEDEFAULT,
                          500,
                          500,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);

    glFrustum(-1,1, -1,1, 2,80);
    glEnable(GL_DEPTH_TEST);    //тест на глубину

    glMatrixMode(GL_PROJECTION);    //матрица проекций
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.2, 1000);
    glMatrixMode(GL_MODELVIEW); //матрица моделей
    glLoadIdentity();

    glEnable(GL_LIGHTING);  //включение света
    glEnable(GL_LIGHT0);    //источник света
    glEnable(GL_COLOR_MATERIAL);    //активация освещения
    glEnable(GL_NORMALIZE); //нормализация вектора

    Init();

    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */

            glClearColor(0.1f, 0.0f, 0.1f, 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //тестирование на глубину


            glPushMatrix();
                MoveCamera();
                ShowWorld();
                glPushMatrix(); //вложенная матрица
                    glRotatef(24,0,0,1);
                    glRotatef(theta,0,1,0);
                    float position[] = {0,0,1,0};//вектор освещение сверху вниз
                    glLightfv(GL_LIGHT0, GL_POSITION, position);

                    glTranslatef(0,0,40);   //положение освещения
                    glScalef(1,1,1);    //размер освещения
                    glColor3f(1,1,1);   //цвет освещения
                    Draw();
                glPopMatrix();
                glColor3f(0,0,1);
                DrawCube();
                glTranslatef(-13.0, 5.0, 1.0);
                float transparency = 0.1;
                for (int i = 0; i < 10; i++)
                {
                    glPushMatrix();
                        glRotatef(36*i, 0, 0, 1);
                        glTranslatef(5, 0, 0);
                        glColor4f(0.1,1,0,transparency);
                        DrawPrism(11);
                    glPopMatrix();
                    transparency += 0.1;
                }
            glPopMatrix();

            theta++;
            SwapBuffers(hDC);

            Sleep (1);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
        {
            switch (wParam)
            {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                break;
            }
        }
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
    PIXELFORMATDESCRIPTOR pfd;

    int iFormat;

    /* get the device context (DC) */
    *hDC = GetDC(hwnd);

    /* set the pixel format for the DC */
    ZeroMemory(&pfd, sizeof(pfd));

    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW |
                  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    iFormat = ChoosePixelFormat(*hDC, &pfd);

    SetPixelFormat(*hDC, iFormat, &pfd);

    /* create and enable the render context (RC) */
    *hRC = wglCreateContext(*hDC);

    wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL (HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}

