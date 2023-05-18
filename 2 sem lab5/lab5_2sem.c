#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define _USE_MATH_DEFINES
#include<math.h>
#define IDC_BUTTON 3456
#define IDC_BUTTON2 3457
#define vertices 11

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** A,
    HPEN KPen, HPEN PPen, HDC hdc);
void arrow(double fi, double px, double py, HDC hdc);
void draw_arc(int x1, int y1, int x2, int y2, int distance, HDC hdc);
void print_matrix(double** matrix, int rows, int columns, int startX, int startY, HDC hdc);

double** init_double_matrix(int rows, int columns);
void fill_zero(double** matrix, int rows, int columns);
double** randm(int rows, int columns);
double** mulmr(double k, double** matrix, int rows, int columns);
void free_all(double** matrix, int rows);

int find_arcs(double** matrix, int rows, int columns);
void dfs(int vertex, double** matrix, int* visited, int* n, int* dfs_vertices, double** dfs_tree);
void start_dfs(int vertex, double** matrix, int* visited, int n, int* dfs_vertices, double** dfs_tree);
void bfs(double** matrix, int start, int* queue, double** bfs_tree);
double** traversal_nums(int* arr);
void tmp_tree_matrix(double** tree, int vertex, double** helpm_bfs);

struct coords
{
    double nx[vertices];
    double ny[vertices];
    double loop_X[vertices];
    double loop_Y[vertices];
    double num_x[vertices];
    double num_y[vertices];
};

int n_dfs = 0;
int n_bfs = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    WNDCLASS w;
    w.lpszClassName = L"Lab 5";
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
    hWnd = CreateWindow(L"Lab 5",
        L"Lab 5. Made by Mariia Kryvokhata",
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
    int b;
    while ((b = GetMessage(&lpMsg, hWnd, 0, 0)) != 0) {
        if (b == -1) {
            return lpMsg.wParam;
        }
        else {
            TranslateMessage(&lpMsg);
            DispatchMessage(&lpMsg);
        }
    }
    return(lpMsg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    HWND hwndButton_dfs;
    HWND hwndButton_bfs;
    int flag_dfs = 0;
    int flag_bfs = 0;
    double** T = randm(vertices, vertices);
    double coef = 1.0 - 0.01 - 0.005 - 0.15;
    double** A = mulmr(coef, T, vertices, vertices);

    int* visited = malloc(vertices * sizeof(int));
    int* dfs_vertices = malloc(vertices * sizeof(int));
    int* queue = malloc(vertices * sizeof(int));
    int start_v = find_arcs(A, vertices, vertices);
    double** dfs_tree = init_double_matrix(vertices, vertices);
    double** bfs_tree = init_double_matrix(vertices, vertices);
    fill_zero(dfs_tree, vertices, vertices);
    fill_zero(bfs_tree, vertices, vertices);

    start_dfs(start_v, A, visited, 0, dfs_vertices, dfs_tree);
    bfs(A, start_v, queue, bfs_tree);

    double** dfs_nums = traversal_nums(dfs_vertices);
    double** bfs_nums = traversal_nums(queue);
    switch (messg) {
    case WM_CREATE:
    {
        hwndButton_dfs = CreateWindow(
            L"BUTTON",
            L"Go dfs",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            700,
            50,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        hwndButton_bfs = CreateWindow(
            L"BUTTON",
            L"Go bfs",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            700,
            125,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON2,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);

        for (int i = 0; i < vertices; i++)
        {
            wchar_t buffer[5];
            swprintf(buffer, 5, L"%d ", dfs_vertices[i] + 1);
            OutputDebugString(buffer);
        }
        for (int i = 0; i < vertices; i++)
        {
            wchar_t buffer[5];
            swprintf(buffer, 5, L"%d ", queue[i] + 1);
            OutputDebugString(buffer);
        }
        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON:
            flag_dfs = 1;
            if (n_dfs < vertices)
            {
                n_dfs++;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            break;

        case IDC_BUTTON2:
            flag_bfs = 1;
            if (n_bfs < vertices)
            {
                n_bfs++;
            }
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
        HPEN RPen = CreatePen(PS_SOLID, 2, RGB(219, 56, 56));
        HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

        SelectObject(hdc, NoPen);
        Rectangle(hdc, 0, 0, 670, 700);

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
            coords.num_x[i] = centerX - 20 + (rad * 1.25 + 2) * sin_angle;
            coords.num_y[i] = centerY - (rad * 1.25 + 2) * cos_angle;
        }

        //PRINT RANDOM MATRIX
        int startX_rm = 700;
        int startY_rm = 200;
        TextOut(hdc, startX_rm, startY_rm, L"Random matrix", 13);
        print_matrix(A, vertices, vertices, startX_rm, startY_rm, hdc);

        //PRINT DFS NUMS MATRIX AND TREE MATRIX
        TextOut(hdc, startX_rm + 200, startY_rm, L"DFS nums matrix", 15);
        print_matrix(dfs_nums, vertices, vertices, startX_rm + 200, startY_rm, hdc);
        TextOut(hdc, startX_rm + 400, startY_rm, L"DFS tree matrix", 15);
        print_matrix(dfs_tree, vertices, vertices, startX_rm + 400, startY_rm, hdc);

        //PRINT BFS NUMS MATRIX AND TREE MATRIX
        TextOut(hdc, startX_rm + 200, startY_rm + 200, L"BFS nums matrix", 15);
        print_matrix(bfs_nums, vertices, vertices, startX_rm + 200, startY_rm + 200, hdc);
        TextOut(hdc, startX_rm + 400, startY_rm + 200, L"BFS tree matrix", 15);
        print_matrix(bfs_tree, vertices, vertices, startX_rm + 400, startY_rm + 200, hdc);

        //DRAW GRAPH
        SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));
        SelectObject(hdc, KPen);
        draw_directgraph(centerX, centerY, rad, vertex_rad, loop_rad, coords, A, KPen, PPen, hdc);

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

        SelectObject(hdc, RPen);
        SetDCBrushColor(hdc, RGB(219, 56, 56));
        if (flag_dfs == 1)
        {
            double** helpm_dfs = init_double_matrix(vertices, vertices);
            fill_zero(helpm_dfs, vertices, vertices);
            for (int i = 0; i < n_dfs; i++)
            {
                tmp_tree_matrix(dfs_tree, dfs_vertices[i], helpm_dfs);
                draw_directgraph(centerX, centerY, rad, vertex_rad, loop_rad, coords, helpm_dfs, RPen, PPen, hdc);

                Ellipse(hdc, coords.nx[dfs_vertices[i]] - vertex_rad, coords.ny[dfs_vertices[i]] - vertex_rad, coords.nx[dfs_vertices[i]] + vertex_rad, coords.ny[dfs_vertices[i]] + vertex_rad);
                wchar_t buffer2[10];
                swprintf(buffer2, 10, L"DFS: %d ", i + 1);
                TextOut(hdc, coords.num_x[dfs_vertices[i]], coords.num_y[dfs_vertices[i]], buffer2, 7);
            }
            for (int i = 0; i < n_dfs; i++)
            {
                wchar_t buffer[5];
                swprintf(buffer, 5, L"%d", dfs_vertices[i] + 1);
                TextOut(hdc, coords.nx[dfs_vertices[i]] - dtx, coords.ny[dfs_vertices[i]] - vertex_rad / 2, buffer, 2);
            }
            free_all(helpm_dfs, vertices);
        }
        if(flag_bfs == 1)
        {
            double** helpm_bfs = init_double_matrix(vertices, vertices);
            fill_zero(helpm_bfs, vertices, vertices);
            for (int i = 0; i < n_bfs; i++)
            {
                tmp_tree_matrix(bfs_tree, queue[i], helpm_bfs);
                draw_directgraph(centerX, centerY, rad, vertex_rad, loop_rad, coords, helpm_bfs, RPen, PPen, hdc);

                Ellipse(hdc, coords.nx[queue[i]] - vertex_rad, coords.ny[queue[i]] - vertex_rad, coords.nx[queue[i]] + vertex_rad, coords.ny[queue[i]] + vertex_rad);

                wchar_t buffer2[10];
                swprintf(buffer2, 10, L"BFS: %d ", i + 1);
                TextOut(hdc, coords.num_x[queue[i]], coords.num_y[queue[i]], buffer2, 7);
            }
            for (int i = 0; i < n_bfs; i++)
            {
                wchar_t buffer[5];
                swprintf(buffer, 5, L"%d", queue[i] + 1);
                TextOut(hdc, coords.nx[queue[i]] - dtx, coords.ny[queue[i]] - vertex_rad / 2, buffer, 2);
            }
            free_all(helpm_bfs, vertices);
        }
        EndPaint(hWnd, &ps);
        break;

    case WM_DESTROY:
        free_all(A, vertices);
        free(visited);
        free(dfs_vertices);
        free(queue);
        free_all(dfs_nums, vertices);
        free_all(bfs_nums, vertices);
        free_all(dfs_tree, vertices);
        free_all(bfs_tree, vertices);
        PostQuitMessage(0);
        break;
    default:
        return(DefWindowProc(hWnd, messg, wParam, lParam));
        break;
    }
    return 0;
}

void draw_directgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** A,
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
                    double line_angle = atan2(coords.ny[i] - coords.ny[j], coords.nx[i] - coords.nx[j]);
                    double x = coords.nx[j] + vertex_rad * cos(line_angle);
                    double y = coords.ny[j] + vertex_rad * sin(line_angle);
                    LineTo(hdc, x, y);
                    arrow(line_angle, x, y, hdc);
                }
            }
            else if (j < i && A[i][j] == 1 && A[j][i] == 1)
            {
                draw_arc(coords.nx[i], coords.ny[i], coords.nx[j], coords.ny[j], fabs(i - j), hdc);
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
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = (double*)malloc(columns * sizeof(double));
    }
    return matrix;
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

double** randm(int rows, int columns)
{
    srand(2111);
    double** matrix = init_double_matrix(rows, columns);
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

void dfs(int vertex, double** matrix, int* visited, int* n, int* dfs_vertices, double** dfs_tree)
{
    visited[vertex] = 1;
    dfs_vertices[(*n)++] = vertex;
    for (int i = 0; i < vertices; i++)
    {
        if (matrix[vertex][i] == 1 && visited[i] == 0)
        {
            dfs_tree[vertex][i] = 1;
            dfs(i, matrix, visited, n, dfs_vertices, dfs_tree);
        }
    }
}

void start_dfs(int vertex, double** matrix, int* visited, int n, int* dfs_vertices, double** dfs_tree)
{
    for (int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    for (int i = 0; i < vertices; i++)
    {
        if (visited[i] == 0)
        {
            dfs(vertex, matrix, visited, &n, dfs_vertices, dfs_tree);
        }
    }
}

int find_arcs(double** matrix, int rows, int columns)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (matrix[i][j] == 1 && matrix[j][i] == 1)
            {
                return i;
            }
        }
    }
    return 0;
}

void bfs(double** matrix, int start, int* queue, double** bfs_tree)
{
    int visited[vertices];
    for (int i = 0; i < vertices; i++)
    {
        visited[i] = 0;
    }
    int front = 0;
    int rear = -1;

    queue[++rear] = start;
    visited[start] = 1;

    while (front <= rear) {
        int current = queue[front++];
        for (int i = 0; i < vertices; i++) {
            if (matrix[current][i] == 1 && visited[i] == 0)
            {
                bfs_tree[current][i] = 1;
                queue[++rear] = i;
                visited[i] = 1;
            }
        }
    }
}

double** traversal_nums(int* arr)
{
    double** nums = init_double_matrix(vertices, vertices);
    fill_zero(nums, vertices, vertices);
    //row is vertex
    //column is number
    for (int i = 0; i < vertices; i++)
    {
        nums[arr[i]][i] = 1;
    }
    return nums;
}

void tmp_tree_matrix(double** tree, int vertex, double** helpm_bfs)
{
    for (int i = 0; i < vertices; i++)
    {
        if (tree[i][vertex] == 1)
        {
            helpm_bfs[i][vertex] = 1;
        }
    }
}
