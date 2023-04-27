#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define _USE_MATH_DEFINES
#include<math.h>
#define IDC_BUTTON 3456
#define IDC_BUTTON2 3457
#define IDC_BUTTON3 3458
#define IDC_BUTTON4 3459
#define vertices 11

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, double angle, struct coords coords, double** A,
    HPEN KPen, HPEN PPen, HDC hdc);
void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** B,
    HPEN KPen, HPEN PPen, HDC hdc);
void arrow(double fi, double px, double py, HDC hdc);
void draw_arc(int x1, int y1, int x2, int y2, int distance, HDC hdc);
void print_matrix(double** matrix, int rows, int columns, int startX, int startY, HDC hdc);

double** randm(int rows, int columns);
double** mulmr(double k, double** matrix, int rows, int columns);
double** symetricm(double** matrix, int rows, int columns);
void free_all(double** matrix, int rows);

double** init_double_matrix(int rows, int columns);

int* power_undirect(double** matrix, int* powers_symetric, int rows, int columns, struct coords coords, HDC hdc);
int* power_entry(double** matrix, int* powers_entry, int rows, int columns, struct coords coords, HDC hdc);
int* power_exit(double** matrix, int* powers_exit, int rows, int columns, struct coords coords, HDC hdc);
void isolated_undirect(int* powers, int x, int y, HDC hdc);
void hanging_undirect( int* powers, int x, int y, HDC hdc);
void isolated_direct(int* powers_entry, int* powers_exit, int x, int y, HDC hdc);
void hanging_direct(int* powers_entry, int* powers_exit, int x, int y, HDC hdc);
int same_power( int* powers_symetric, int x, int y, HDC hdc);

void fill_zero(double** matrix, int rows, int columns);
double** multiply_matrix(double** composed, double** matrix1, double** matrix2, int columns, int rows);
void copy_matrix(double** matrix1, double** matrix2, int rows, int columns);
void logic_or(double** result, double** matrix1, double** matrix2, int rows, int columns);
void strongroute_matrix(double** matrix, int rows, int columns);
void print_paths_2(double** paths2, double** matrix, int rows, int columns);
void print_paths_3(double** paths3, double** matrix, int rows, int columns);

void draw_condens_graph(int components_num, double** C, int vertex_rad, double dtx, struct coords coords, HPEN BPen, HDC hdc);
void dfs(int vertex, double** matrix, int* visited, int* stack, int* v);
void dfs_transpose(int vertex, double** transposed_matrix, int* visited, int components_num, int* component_labels);
int print_components(int* visited, double** transposed_matrix, int* stack, int* component_labels, int* v);
void fill_condensed_matrix(double** matrix, double** condensed_matrix, int* component_labels);
int find_components(int* visited, double** matrix, double** transposed_matrix, double** condensed_matrix,
                     int* stack, int* component_labels, int v);

char ProgName[] = "Lab 4";

struct coords
{
    double nx[vertices];
    double ny[vertices];
    double condensX[vertices];
    double condensY[vertices];
    double loop_X[vertices];
    double loop_Y[vertices];
    double power_x[vertices];
    double power_y[vertices];
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
        L"Lab 4. Made by Mariia Kryvokhata",
        WS_OVERLAPPEDWINDOW,
        100,
        100,
        1300,
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
    HWND hwndButton_modif;
    HWND hwndButton_condens;
    int graph_flag = 1;
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
        hwndButton_modif = CreateWindow(
            L"BUTTON",
            L"Modified graph",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            900,
            50,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON3,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        hwndButton_condens = CreateWindow(
            L"BUTTON",
            L"Condensed graph",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            900,
            125,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON4,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {

        case IDC_BUTTON:
            graph_flag = 0;
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case IDC_BUTTON2:
            graph_flag = 1;
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case IDC_BUTTON3:
            graph_flag = 2;
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case IDC_BUTTON4:
            graph_flag = 3;
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
        Rectangle(hdc, 0, 0, 695, 700);

        //set parameters of the graph
        struct coords coords;
        double rad = 200;
        double vertex_rad = rad / 10;
        double loop_rad = vertex_rad;
        double dtx = vertex_rad / 2.5;
        double centerX = 350;
        double centerY = 350;
        double angle = 2.0 * M_PI / (double)vertices;
        for (int i = 0; i < vertices; i++)
        {
            double sin_angle = sin(angle * (double)i);
            double cos_angle = cos(angle * (double)i);
            coords.nx[i] = centerX + rad * sin_angle;
            coords.ny[i] = centerY - rad * cos_angle;
            coords.loop_X[i] = centerX + (rad + loop_rad) * sin_angle;
            coords.loop_Y[i] = centerY - (rad + loop_rad) * cos_angle;
            coords.power_x[i] = centerX + (rad + 50) * sin_angle;
            coords.power_y[i] = centerY - (rad + 50) * cos_angle;
        }

        //RANDOM MATRIX
        double** T = randm(vertices, vertices);
        double coef = 1.0 - 0.01 - 0.01 - 0.3;
        double** A = mulmr(coef, T, vertices, vertices);
        int startX_rm = 700;
        int startY_rm = 200;
        TextOut(hdc, startX_rm, startY_rm, L"Random matrix", 13);
        print_matrix(A, vertices, vertices, startX_rm, startY_rm, hdc);

        //SYMETRIC MATRIX
        double** T2 = randm(vertices, vertices);
        double** B = symetricm(mulmr(coef, T2, vertices, vertices), vertices, vertices);
        int startX_sm = startX_rm;
        int startY_sm = startY_rm + 200;
        TextOut(hdc, startX_sm, startY_sm, L"Symetric matrix", 15);
        print_matrix(B, vertices, vertices, startX_sm, startY_sm, hdc);

        //MODIFIED MATRIX
        double** T3 = randm(vertices, vertices);
        double coef_modif = 1.0 - 0.005 - 0.005 - 0.27;
        double** M = mulmr(coef_modif, T3, vertices, vertices);
        int startX_mm = startX_rm + 200;
        int startY_mm = startY_rm;
        TextOut(hdc, startX_mm, startY_mm, L"Modified matrix", 15);
        print_matrix(M, vertices, vertices, startX_rm + 200, startY_rm, hdc);

        //COMPOSED
        //PATHS WITH DISTANCE 2
        double** paths2 = init_double_matrix(vertices, vertices);
        fill_zero(paths2, vertices, vertices);
        paths2 = multiply_matrix(paths2, M, M, vertices, vertices);
        //TextOut(hdc, startX_mm + 200, startY_mm, L"Paths with distance 2", 21);
        //print_matrix(composed, vertices, vertices, startX_mm + 200, startY_mm, hdc);

        //PATHS WITH DISTANCE 3
        double** paths3 = init_double_matrix(vertices, vertices);
        fill_zero(paths3, vertices, vertices);
        paths3 = multiply_matrix(paths3, paths2, M, vertices, vertices);
        //TextOut(hdc, startX_mm + 200, startY_mm + 200, L"Paths with distance 3", 21);
        //print_matrix(composed2, vertices, vertices, startX_mm + 200, startY_mm + 200, hdc);

        //транзитивне замикання
        double** reach_matrix = init_double_matrix(vertices, vertices);
        double** tmp = init_double_matrix(vertices, vertices);
        double** result = init_double_matrix(vertices, vertices);
        fill_zero(reach_matrix, vertices, vertices);
        reach_matrix = multiply_matrix(reach_matrix, M, M, vertices, vertices);
        for (int i = 0; i < vertices - 1; i++)
        {
            copy_matrix(tmp, reach_matrix, vertices, vertices);
            reach_matrix = multiply_matrix(reach_matrix, reach_matrix, M, vertices, vertices);
            logic_or(result, tmp, reach_matrix, vertices, vertices);
        }
        TextOut(hdc, startX_mm, startY_mm + 200, L"Reach matrix", 13);
        print_matrix(result, vertices, vertices, startX_mm, startY_mm + 200, hdc);

        strongroute_matrix(result, vertices, vertices);
        TextOut(hdc, startX_mm + 200, startY_mm + 200, L"Connection matrix", 17);
        print_matrix(result, vertices, vertices, startX_mm + 200, startY_mm + 200, hdc);

        //DRAW GRAPH
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        SelectObject(hdc, KPen);
        if(graph_flag == 1)
        {
            draw_undirectgraph(centerX, centerY, rad, vertex_rad, loop_rad, coords, B, KPen, PPen, hdc);
            int* powers_undirect = malloc(sizeof(int) * vertices);
            powers_undirect = power_undirect(B, powers_undirect, vertices, vertices, coords, hdc);

            TextOut(hdc, 50, 75, L"Isolated: ", 10);
            isolated_undirect(powers_undirect, 75, 75, hdc);

            TextOut(hdc, 50, 90, L"Hanging: ", 10);
            hanging_undirect(powers_undirect, 75, 90, hdc);

            same_power(powers_undirect, 50, 50, hdc);

            free(powers_undirect);
        }
        else if(graph_flag == 3)
        {
            //strong components
            int* visited = malloc(sizeof(int) * vertices);
            int* stack = malloc(sizeof(int) * vertices);
            int* component_labels = malloc(sizeof(int) * vertices);
            double** transposed_matrix = init_double_matrix(vertices, vertices);
            double** condensed_matrix = init_double_matrix(vertices, vertices);
            fill_zero(condensed_matrix, vertices, vertices);
            int components_num = find_components(visited, M, transposed_matrix, condensed_matrix, stack, component_labels, 0);
            TextOut(hdc, 20, 90, L"Condensed matrix", 16);
            print_matrix(condensed_matrix, components_num, components_num, 20, 100, hdc);
            wchar_t buffer[5];
            swprintf(buffer, 5, L"%d ", components_num);
            OutputDebugString(buffer);

            //coords
            double comp_angle = 2.0 * M_PI / (double)components_num;
            for (int i = 0; i < components_num; i++)
            {
                double sin_angle = sin(comp_angle * (double)i);
                double cos_angle = cos(comp_angle * (double)i);
                coords.condensX[i] = centerX + rad * sin_angle;
                coords.condensY[i] = centerY - rad * cos_angle;
            }

            //draw graph
            draw_condens_graph(components_num, condensed_matrix, vertex_rad, dtx, coords, BPen, hdc);

            EndPaint(hWnd, &ps);
            free(visited);
            free(stack);
            free(component_labels);
            free_all(transposed_matrix, vertices);
            free_all(condensed_matrix, vertices);
        }
        else
        {
            double** matrix;
            if (graph_flag == 0)
            {
                matrix = A;
            }
            else
            {
                print_paths_2(paths2, M, vertices, vertices);
                print_paths_3(paths3, M, vertices, vertices);
                matrix = M;
            }
            draw_directgraph(centerX, centerY, rad, vertex_rad, loop_rad, angle, coords, matrix, KPen, PPen, hdc);

            int* powers_entry = malloc(sizeof(int) * vertices);
            powers_entry = power_entry(matrix, powers_entry, vertices, vertices, coords, hdc);

            int* powers_exit = malloc(sizeof(int) * vertices);
            powers_exit = power_exit(matrix, powers_exit, vertices, vertices, coords, hdc);

            TextOut(hdc, 50, 75, L"Isolated: ", 10);
            isolated_direct(powers_entry, powers_exit, 75, 75, hdc);
            TextOut(hdc, 50, 90, L"Hanging: ", 10);
            hanging_direct(powers_entry, powers_exit, 75, 90, hdc);

            free(powers_entry);
            free(powers_exit);
        }

        //draw vertices
        SelectObject(hdc, BPen);
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(204, 204, 255));
        SetBkMode(hdc, TRANSPARENT);
        for (int i = 0; i < vertices; i++) {
            Ellipse(hdc, coords.nx[i] - vertex_rad, coords.ny[i] - vertex_rad, coords.nx[i] + vertex_rad, coords.ny[i] + vertex_rad);
            wchar_t buffer[5];
            swprintf(buffer, 5, L"%d", i + 1);
            TextOut(hdc, coords.nx[i] - dtx, coords.ny[i] - vertex_rad / 2, buffer, 2);
        }

        free_all(A, vertices);
        free_all(B, vertices);
        free_all(M, vertices);
        free_all(paths2, vertices);
        free_all(paths3, vertices);
        free_all(reach_matrix, vertices);
        free_all(result, vertices);
        free_all(tmp, vertices);

        EndPaint(hWnd, &ps);
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

void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, double angle, struct coords coords, double** A,
    HPEN KPen, HPEN PPen, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            MoveToEx(hdc, coords.nx[i], coords.ny[i], NULL);
            if ((j >= i && A[i][j] == 1) || (j <= i && A[i][j] == 1 && A[j][i] == 0))
            {
                if (i == j)
                {
                    SelectObject(hdc, PPen);
                    //draw loops
                    Ellipse(hdc, coords.loop_X[i] - loop_rad, coords.loop_Y[i] - loop_rad, coords.loop_X[i] + loop_rad, coords.loop_Y[i] + loop_rad);

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

void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** B,
    HPEN KPen, HPEN PPen, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            MoveToEx(hdc, coords.nx[i], coords.ny[i], NULL);
            if (B[i][j] == 1)
            {
                if (i == j)
                {
                    //draw loops
                    SelectObject(hdc, PPen);
                    Ellipse(hdc, coords.loop_X[i] - loop_rad, coords.loop_Y[i] - loop_rad, coords.loop_X[i] + loop_rad, coords.loop_Y[i] + loop_rad);
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

void draw_condens_graph(int components_num, double** C, int vertex_rad, double dtx, struct coords coords, HPEN BPen, HDC hdc)
{
    for (int i = 0; i < components_num; i++)
    {
        for (int j = 0; j < components_num; j++)
        {
            if (C[i][j] == 1.)
            {
                MoveToEx(hdc, coords.condensX[i], coords.condensY[i], NULL);
                LineTo(hdc, coords.condensX[j], coords.condensY[j]);
                double line_angle = atan2(coords.condensY[i] - coords.condensY[j], coords.condensX[i] - coords.condensX[j]);
                arrow(line_angle, coords.condensX[j] + vertex_rad * cos(line_angle), coords.condensY[j] + vertex_rad * sin(line_angle), hdc);
            }
        }
    }
    SelectObject(hdc, BPen);
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(204, 204, 255));
    SetBkMode(hdc, TRANSPARENT);
    for (int i = 0; i < components_num; i++) {
        Ellipse(hdc, coords.condensX[i] - vertex_rad, coords.condensY[i] - vertex_rad, coords.condensX[i] + vertex_rad, coords.condensY[i] + vertex_rad);
        wchar_t buffer[5];
        swprintf(buffer, 5, L"%d", i + 1);
        TextOut(hdc, coords.condensX[i] - dtx, coords.condensY[i] - vertex_rad / 2, buffer, 2);
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
    double component = b * sqrt(b * b * vertex_rad * vertex_rad - a * a * b * b + a * a * ellipse_y0 * ellipse_y0 - a * a * vertex_rad * vertex_rad + pow(a, 4));
    double contact_y1 = (a * a * ellipse_y0 - component) / (-b * b + a * a);
    double contact_x1 = sqrt(vertex_rad * vertex_rad - contact_y1 * contact_y1);
    double contact_y2 = length - contact_y1;
    double contact_x2 = -contact_x1;
    if (distance <= vertices / 2)
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

double** init_double_matrix(int rows, int columns)
{
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < vertices; i++)
    {
        matrix[i] = (double*)calloc(columns, sizeof(double));
    }
    return matrix;
}

double** randm(int rows, int columns)
{
    srand(2111);
    double** matrix = init_double_matrix(vertices, vertices);
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
            matrix[i][j] = matrix[i][j] * k;
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

void print_matrix(double** matrix, int rows, int columns, int startX, int startY, HDC hdc)
{
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

int* power_undirect(double** matrix, int* powers_symetric, int rows, int columns, struct coords coords, HDC hdc)
{
    for (int i = 0; i < rows; i++)
    {
        powers_symetric[i] = 0;
        for (int j = 0; j < columns; j++)
        {
            if (matrix[i][j] == 1)
            {
                if (i == j)
                {
                    powers_symetric[i] += 2;
                }
                else
                {
                    powers_symetric[i]++;
                }
            }
        }
        wchar_t buffer[15];
        swprintf(buffer, 9, L"Power: %d", powers_symetric[i]);
        TextOut(hdc, coords.power_x[i], coords.power_y[i], buffer, 9);
    }
    return powers_symetric;
}

int* power_entry(double** matrix, int* powers_entry, int rows, int columns, struct coords coords, HDC hdc)
{
    for (int j = 0; j < columns; j++)
    {
        powers_entry[j] = 0;
        for (int i = 0; i < rows; i++)
        {
            if (matrix[i][j] == 1)
            {
                powers_entry[j]++;
            }
        }
        wchar_t buffer[20];
        swprintf(buffer, 15, L"Power entry: %d", powers_entry[j]);
        TextOut(hdc, coords.power_x[j], coords.power_y[j], buffer, 15);
    }
    return powers_entry;
}

int* power_exit(double** matrix, int* powers_exit, int rows, int columns, struct coords coords, HDC hdc)
{
    for (int i = 0; i < rows; i++)
    {
        powers_exit[i] = 0;
        for (int j = 0; j < columns; j++)
        {
            if (matrix[i][j] == 1)
            {
                powers_exit[i]++;
            }
        }
        wchar_t buffer[20];
        swprintf(buffer, 14, L"Power exit: %d", powers_exit[i]);
        TextOut(hdc, coords.power_x[i], coords.power_y[i] + 20, buffer, 14);
    }
    return powers_exit;
}

void isolated_undirect(int* powers, int x, int y, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        if (powers[i] == 0)
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%d ", i);
            TextOut(hdc, x, y, buffer, 2);
        }
    }
}

void hanging_undirect(int* powers, int x, int y, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        if (powers[i] == 1)
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%d", i);
            TextOut(hdc, x, y, buffer, 2);
        }
    }
}

void isolated_direct(int* powers_entry, int* powers_exit, int x, int y, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        if (powers_entry[i] == 0 && powers_exit[i] == 0)
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%d ", i);
            TextOut(hdc, x, y, buffer, 2);
        }
    }
}

void hanging_direct(int* powers_entry, int* powers_exit, int x, int y, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        if ((powers_entry[i] == 1 && powers_exit[i] == 0) || (powers_entry[i] == 0 && powers_exit[i] == 1))
        {
            wchar_t buffer[2];
            swprintf(buffer, 2, L"%d ", i);
            TextOut(hdc, x, y, buffer, 2);
        }
    }
}

int same_power(int* powers, int x, int y, HDC hdc)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = i + 1; j < vertices; j++)
        {
            if (powers[i] != powers[j])
            {
                TextOut(hdc, x, y, L"Different powers", 17);
                return 0;
            }
        }
    }
    wchar_t buffer[20];
    swprintf(buffer, 15, L"Same power: %d", powers[0]);
    TextOut(hdc, x, y, buffer, 15);
    return 0;
}

double** multiply_matrix(double** composed, double** matrix1, double** matrix2, int columns, int rows)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            for (int k = 0; k < columns; k++)
            {
                composed[i][j] += matrix1[i][k] * matrix2[k][j];
            }
            if (composed[i][j] != 0.)
            {
                composed[i][j] = 1.;
            }
        }
    }
    return composed;
}

void fill_zero(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix[i][j] = 0.;
        }
    }
}

void print_paths_2(double** paths2, double** matrix, int rows, int columns)
{
    OutputDebugString(L"Paths with distance 2\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (paths2[i][j] == 1.)
            {
                for (int k = 0; k < columns; k++)
                {
                    if (matrix[i][k] == 1 && matrix[k][j] == 1)
                    {
                        wchar_t buffer[25];
                        swprintf(buffer, 25, L"%d -> %d -> %d\n", i + 1, k + 1, j + 1);
                        OutputDebugString(buffer);
                    }
                }
            }
        }
    }
}

void print_paths_3(double** paths3, double** matrix, int rows, int columns)
{
    OutputDebugString(L"Paths with distance 3\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (paths3[i][j] == 1.)
            {
                for (int k = 0; k < columns; k++)
                {
                    for (int m = 0; m < columns; m++)
                    {
                        if (matrix[i][k] == 1 && matrix[k][m] == 1 && matrix[m][j])
                        {
                            wchar_t buffer[25];
                            swprintf(buffer, 25, L"%d -> %d -> %d -> %d\n", i + 1, k + 1, m + 1, j + 1);
                            OutputDebugString(buffer);
                        }
                    }
                }
            }
        }
    }
}

void copy_matrix(double** matrix1, double** matrix2, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            matrix1[i][j] = matrix2[i][j];
        }
    }
}

//MAYBE OPTIMIZE??
void logic_or(double** result, double** matrix1, double** matrix2, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if ((matrix1[i][j] == 0 && matrix2[i][j] == 1) || 
                (matrix1[i][j] == 1 && matrix2[i][j] == 0) ||
                (matrix1[i][j] == 1 && matrix2[i][j] == 1))
            {
                result[i][j] = 1.;
            }
            else
            {
                result[i][j] = 0.;
            }
        }
    }
}

void strongroute_matrix(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            if (matrix[i][j] != matrix[j][i])
            {
                matrix[i][j] = 0.;
                matrix[j][i] = 0.;
            }
        }
    }
}

void dfs(int vertex, double** matrix, int* visited, int* stack, int* v)
{
    visited[vertex] = 1;
    for (int i = 0; i < vertices; i++)
    {
        if (matrix[vertex][i] && !visited[i])
        {
            dfs(i, matrix, visited, stack, v);
        }
    }
    stack[(*v)++] = vertex;
}

void dfs_transpose(int vertex, double** transposed_matrix, int* visited, int components_num, int* component_labels)
{
    visited[vertex] = 1;
    component_labels[vertex] = components_num;
    wchar_t buffer[5];
    swprintf(buffer, 5, L"%d ", vertex + 1);
    OutputDebugString(buffer);
    for (int i = 0; i < vertices; i++)
    {
        if (transposed_matrix[vertex][i] && !visited[i])
        {
            dfs_transpose(i, transposed_matrix, visited, components_num, component_labels);
        }
    }
}

int print_components(int* visited, double** transposed_matrix, int* stack, int* component_labels, int* v)
{
    int components_num = 0;
    for (int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    while ((*v) > 0)
    {
        int vertex = stack[--(*v)];
        if (!visited[vertex])
        {
            components_num++;
            OutputDebugString(L"Component: ");
            dfs_transpose(vertex, transposed_matrix, visited, components_num, component_labels);
            OutputDebugString(L"\n");
        }
    }
    return components_num;
}

void fill_condensed_matrix(double** matrix, double** condensed_matrix, int* component_labels)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (matrix[i][j])
            {
                if (component_labels[i] != component_labels[j])
                {
                    condensed_matrix[component_labels[i] - 1][component_labels[j] - 1] = 1.;
                }
            }
        }
    }
}

int find_components(int* visited, double** matrix, double** transposed_matrix, double** condensed_matrix, int* stack, int* component_labels, int v)
{
    for (int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    for (int i = 0; i < vertices; i++)
    {
        if (!visited[i])
        {
            dfs(i, matrix, visited, stack, &v);
        }
    }
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            transposed_matrix[i][j] = matrix[j][i];
        }
    }
    int* components_num = print_components(visited, transposed_matrix, stack, component_labels, &v, condensed_matrix);
    fill_condensed_matrix(matrix, condensed_matrix, component_labels);
    return components_num;
}