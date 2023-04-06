//DON'T FORGET TO CHECK LEAKS!!!!
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define _USE_MATH_DEFINES
#include<math.h>
#define IDC_BUTTON 3456
#define IDC_BUTTON2 3457
#define vertexes 11
#define n3 1.0
#define n4 1.0

double** randm(int rows, int columns);
double** mulmr(double k, double** matrix, int rows, int columns);
double** symetricm(double** matrix, int rows, int columns);
void free_all(double** matrix, int rows);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
char ProgName[] = "Lab 3";
void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, double angle, struct coords coords, double** A, 
                      HPEN KPen, HPEN PPen, HDC hdc);
void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, double angle, struct coords coords, double** B, 
                        HPEN KPen, HPEN PPen, HDC hdc);
void arrow(double fi, double px, double py, HDC hdc);
void draw_arc(int x1, int y1, int x2, int y2, int distance, HDC hdc);
void print_mrand(double** matrix, int rows, int columns, int startX, int startY, HDC hdc);
void print_symetricm(double** matrix, int rows, int columns, int startX, int startY, HDC hdc);

struct coords
{
    double nx[vertexes];
    double ny[vertexes];
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS w;
    w.lpszClassName = ProgName;
    w.hInstance = hInstance;
    w.lpfnWndProc = WndProc;
    w.hCursor = LoadCursor(NULL, IDC_ARROW);
    w.hIcon = 0;
    w.lpszMenuName = 0;
    w.hbrBackground = WHITE_BRUSH;
    w.style = CS_HREDRAW | CS_VREDRAW;
    w.cbClsExtra = 0;
    w.cbWndExtra = 0;
    if (!RegisterClass(&w))
    {
        return 0;
    }
    HWND hWnd;
    MSG lpMsg;
    hWnd = CreateWindow(ProgName,
        L"Lab 3. Made by Mariia Kryvokhata",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1000,
        700,
        (HWND)NULL,
        (HMENU)NULL,
        (HINSTANCE)hInstance,
        (HINSTANCE)NULL);
    ShowWindow(hWnd, nCmdShow);
    while (GetMessage(&lpMsg, hWnd, 0, 0))
    {
        TranslateMessage(&lpMsg);
        DispatchMessage(&lpMsg);
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HWND hwndButton_direct;
    HWND hwndButton_undirect;
    int flag = 1;
    switch (messg) {
    case WM_CREATE:
    {
        hwndButton_direct = CreateWindow(
            L"BUTTON",
            L"Directed graph",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            700,
            50,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        hwndButton_undirect = CreateWindow(
            L"BUTTON",
            L"Undirected graph",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            700, 
            125,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON2,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {

        case IDC_BUTTON:
            flag = 0;
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case IDC_BUTTON2:
            flag = 1;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
    }

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SetGraphicsMode(hdc, GM_ADVANCED);

        HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
        HPEN KPen = CreatePen(PS_SOLID, 1, RGB(20, 20, 5));
        HPEN PPen = CreatePen(PS_SOLID, 2, RGB(153, 0, 153));
        HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

        SelectObject(hdc, NoPen);
        Rectangle(hdc, 0, 0, 670, 700);

        wchar_t* nn[vertexes] = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"10", L"11"};
        //set parameters of the graph
        struct coords coords;
        double rad = 200;
        double vertex_rad = rad / 10;
        double dtx = vertex_rad / 2.5;
        double centerX = 350;
        double centerY = 350;
        double angle = 2.0 * M_PI / (double)vertexes;
        for (int i = 0; i < vertexes; i++)
        {
            double a = rad * sin(angle * (double)i);
            double b = rad * cos(angle * (double)i);
            coords.nx[i] = centerX + a;
            coords.ny[i] = centerY - b;
        }
        
        //PRINT RANDOM MATRIX
        double** T = randm(vertexes, vertexes);
        double coef = 1.0 - n3 * 0.02 - n4 * 0.005 - 0.25;
        double** A = mulmr(coef, T, vertexes, vertexes);
        int startX_rm = 700;
        int startY_rm = 200;
        print_mrand(A, vertexes, vertexes, startX_rm, startY_rm, hdc);

        //PRINT SYMETRIC MATRIX
        double** T2 = randm(vertexes, vertexes);
        double** B = symetricm(mulmr(coef, T2, vertexes, vertexes), vertexes, vertexes);
        int startX_sm = startX_rm;
        int startY_sm = startY_rm + 200;
        print_symetricm(B, vertexes, vertexes, startX_sm, startY_sm, hdc);

        //DRAW GRAPH
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        SelectObject(hdc, KPen);
        if (flag == 0)
        {
            draw_directgraph(centerX, centerY, rad, vertex_rad, angle, coords, A, KPen, PPen, hdc);
        }
        else
        {
            draw_undirectgraph(centerX, centerY, rad, vertex_rad, angle, coords, B, KPen, PPen, hdc);
        }

        //draw vertexes
        SelectObject(hdc, BPen);
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(204, 204, 255));
        SetBkMode(hdc, TRANSPARENT);
        for (int i = 0; i < vertexes; i++) {
            Ellipse(hdc, coords.nx[i] - vertex_rad, coords.ny[i] - vertex_rad, coords.nx[i] + vertex_rad, coords.ny[i] + vertex_rad);
            TextOut(hdc, coords.nx[i] - dtx, coords.ny[i] - vertex_rad / 2, nn[i], 2);
        }

        EndPaint(hWnd, &ps);
        free_all(A, vertexes);
        free_all(B, vertexes);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, messg, wParam, lParam));
        break;
    }
    return 0;
}

void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, double angle, struct coords coords, double** A,
                      HPEN KPen, HPEN PPen, HDC hdc)
{
    for (int i = 0; i < vertexes; i++)
    {
        for (int j = 0; j < vertexes; j++)
        {
            MoveToEx(hdc, coords.nx[i], coords.ny[i], NULL);
            if ((j >= i && A[i][j] == 1) || (j <= i && A[i][j] == 1 && A[j][i] == 0))
            {
                if (i == j)
                {
                    SelectObject(hdc, PPen);
                    //draw loops
                    double loop_rad = vertex_rad;
                    double loop_X = centerX + (rad + loop_rad) * sin(angle * (double)i);
                    double loop_Y = centerY - (rad + loop_rad) * cos(angle * (double)i);
                    Ellipse(hdc, loop_X - loop_rad, loop_Y - loop_rad, loop_X + loop_rad, loop_Y + loop_rad);

                    //draw arrows for loops
                    double r = rad + loop_rad / 2.;
                    double contact_h = sqrt(3) * vertex_rad / 2.;
                    double contact_fi = atan2(contact_h, r);
                    double contact_r = sqrt(r * r + contact_h * contact_h);
                    double contact_angle = atan2(coords.ny[i] - centerY, coords.nx[i] - centerX);
                    double contact_X = centerX + contact_r * cos(contact_angle + contact_fi);
                    double contact_Y = centerY + contact_r * sin(contact_angle + contact_fi);
                    double arrow_angle = contact_angle + 0.3 / 2.;
                    arrow(arrow_angle, contact_X, contact_Y, hdc);
                    SelectObject(hdc, KPen);
                }
                else
                {
                    LineTo(hdc, coords.nx[j], coords.ny[j]);
                    double line_angle = atan2(coords.ny[i] - coords.ny[j], coords.nx[i] - coords.nx[j]);
                    arrow(line_angle, coords.nx[j] + vertex_rad * cos(line_angle), coords.ny[j] + vertex_rad * sin(line_angle), hdc);
                }
            }
            else if (j < i && A[i][j] == 1 && A[j][i] == 1)
            {
                draw_arc(coords.nx[i], coords.ny[i], coords.nx[j], coords.ny[j], fabs(i - j), hdc);
            }

        }
    }
}

void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, double angle, struct coords coords, double** B, 
                        HPEN KPen, HPEN PPen, HDC hdc)
{
    for (int i = 0; i < vertexes; i++)
    {
        for (int j = 0; j < vertexes; j++)
        {
            MoveToEx(hdc, coords.nx[i], coords.ny[i], NULL);
            if (B[i][j] == 1)
            {
                if (i == j)
                {
                    //draw loops
                    SelectObject(hdc, PPen);
                    double loop_rad = vertex_rad;
                    double loop_X = centerX + (rad + loop_rad) * sin(angle * (double)i);
                    double loop_Y = centerY - (rad + loop_rad) * cos(angle * (double)i);
                    Ellipse(hdc, loop_X - loop_rad, loop_Y - loop_rad, loop_X + loop_rad, loop_Y + loop_rad);
                    SelectObject(hdc, KPen);
                }
                else
                {
                    LineTo(hdc, coords.nx[j], coords.ny[j]);
                }
            }
        }
    }
}

void arrow(double fi, double px, double py, HDC hdc)
{
    double lx, ly, rx, ry;
    lx = px + 15 * cos(fi + 0.3);
    rx = px + 15 * cos(fi - 0.3);
    ly = py + 15 * sin(fi + 0.3);
    ry = py + 15 * sin(fi - 0.3);
    MoveToEx(hdc, lx, ly, NULL);
    LineTo(hdc, px, py);
    LineTo(hdc, rx, ry);
}

void draw_arc(int x1, int y1, int x2, int y2, int distance, HDC hdc)
{
    XFORM xForm;
    XFORM baseForm;
    GetWorldTransform(hdc, &baseForm);
    double angle = atan2(y2 - y1, x2 - x1) - M_PI / 2.0;
    xForm.eM11 = (FLOAT)cos(angle);
    xForm.eM12 = (FLOAT)sin(angle);
    xForm.eM21 = (FLOAT)(-sin(angle));
    xForm.eM22 = (FLOAT)cos(angle);
    xForm.eDx = (FLOAT)x1;
    xForm.eDy = (FLOAT)y1;
    SetWorldTransform(hdc, &xForm);
    const double k = 0.6;
    double length = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double vertex_rad = 20.0;
    double a = k * length;
    double b = length / 2;
    double ellipse_y0 = b;
    double component = b * sqrt(pow(b * vertex_rad, 2) - pow(a * b, 2) + pow(a * ellipse_y0, 2) - pow(a * vertex_rad, 2) + pow(a, 4));
    double contact_y1 = (a * a * ellipse_y0 - component) / (-b * b + a * a);
    double contact_x1 = sqrt(vertex_rad * vertex_rad - contact_y1 * contact_y1);
    double contact_y2 = length - contact_y1;
    double contact_x2 = -contact_x1;
    if (distance < 6)
    {
        Arc(hdc, -k * length, length, k * length, 0, 0, 0, 0, length);
        double arrow_angle = -atan2(length - contact_y2, contact_x2) + 0.3 / 2;
        arrow(arrow_angle, contact_x2, contact_y2, hdc);
    }
    else
    {
        Arc(hdc, -k * length, length, k * length, 0, 0, length, 0, 0);
        double arrow_angle = -atan2(length - contact_y2, -contact_x2) - 0.3 / 2;
        arrow(arrow_angle, -contact_x2, contact_y2, hdc);
    }
    SetWorldTransform(hdc, &baseForm);
}

//FIX POINTERS
double** randm(int rows, int columns)
{
    srand(2111);
    double** matrix = (double**) malloc(sizeof(double*) * rows);
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*) malloc(sizeof(double) * columns);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double)(rand() * 2.0) / (double)RAND_MAX;
        }
    }
    return matrix;
};

double** mulmr(double k, double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (matrix[i][j] < 1)
            {
                matrix[i][j] = 0;
            }
            else
            {
                matrix[i][j] = 1;
            }
        }
    }
    return matrix;
}

double** symetricm(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (matrix[i][j] != matrix[j][i])
            {
                matrix[i][j] = 1;
                matrix[j][i] = 1;
            }
        }
    }
    return matrix;
}

void free_all(double** matrix, int rows)
{
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void print_mrand(double** matrix, int rows, int columns, int startX, int startY, HDC hdc)
{
    wchar_t buffer1[15];
    swprintf(buffer1, 15, L"Random matrix");
    TextOut(hdc, startX, startY, buffer1, 13);
    for (int i = 0, y = startY + 20; i < rows; i++, y += 15)
    {
        for (int j = 0, x = startX; j < columns; j++, x += 15)
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%lf", matrix[i][j]);
            TextOut(hdc, x, y, buffer, 1);
        }
        MoveToEx(hdc, startX, y, NULL);
    }
}

void print_symetricm(double** matrix, int rows, int columns, int startX, int startY, HDC hdc)
{
    wchar_t buffer2[16];
    swprintf(buffer2, 16, L"Symetric matrix");
    TextOut(hdc, startX, startY, buffer2, 15);
    for (int i = 0, y = startY + 20; i < vertexes; i++, y += 15)
    {
        for (int j = 0, x = startX; j < vertexes; j++, x += 15)
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%lf", matrix[i][j]);
            TextOut(hdc, x, y, buffer, 1);
        }
        MoveToEx(hdc, startX, y, NULL);
    }
}