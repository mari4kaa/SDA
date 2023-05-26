//FREE GRAPH!!!!!!!!!!!
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define _USE_MATH_DEFINES
#include<math.h>
#define IDC_BUTTON 3456
#define vertices 11

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** A, double** W,
    HPEN PPen, HDC hdc);
void print_matrix(double** matrix, int rows, int columns, int startX, int startY, HDC hdc);

double** init_double_matrix(int rows, int columns);
void fill_zero(double** matrix, int rows, int columns);
double** randm(int rows, int columns);
double** mulmr(double k, double** matrix, int rows, int columns);
void free_all(double** matrix, int rows);

void modify_matrix(double** matrix, double** mulmatrix);
double** roundm(double** T);
double** symetricm(double** matrix2);
double** B_matrix(double** Wt);
double** C_matrix(double** B);
double** D_matrix(double** B);
double** Wt_matrix(double** Wt, double** C, double** D, double** Tr);
double** Tr_matrix();

struct Graph* createGraph(double** matrix, double** W);
int idx_of_min(int* weights, int* visited);
void find_mst(struct Graph* graph, struct Edge* mst, double** W);
void print_visited(int src, int dest, float weight);
void draw_mst(int n_tree, struct Edge* mst, struct coords coords, double vertex_rad, double dtx, HDC hdc);

struct coords
{
    double nx[vertices];
    double ny[vertices];
    double loop_X[vertices];
    double loop_Y[vertices];
};

//переробити
struct Graph
{
    struct Node* head[vertices];
};

struct Node
{
    int dest;
    double weight;
    struct Node* next;
};

struct Edge
{
    int src;
    int dest;
    double weight;
};


int n_tree = 0;

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
    HWND hwndButton_spanningTree;
    int flag_tree = 0;

    switch (messg) {
    case WM_CREATE:
    {
        hwndButton_spanningTree = CreateWindow(
            L"BUTTON",
            L"Go tree",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            700,
            50,
            160,
            50,
            hWnd,
            (HMENU)IDC_BUTTON,
            (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
            NULL);
        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON:
            flag_tree = 1;
            if (n_tree < vertices)
            {
                n_tree++;
            }
            InvalidateRect(hWnd, NULL, FALSE);
            break;
        }
    }

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        SetGraphicsMode(hdc, GM_ADVANCED);

        //all matrices
        double** T = randm(vertices, vertices);
        double coef = 1.0 - 0.01 - 0.005 - 0.05;
        T = mulmr(coef, T, vertices, vertices);
        double** A = symetricm(T);

        double** T2 = randm(vertices, vertices);
        modify_matrix(T2, A);
        double** Wt = roundm(T2);

        double** B = B_matrix(Wt);
        double** C = C_matrix(B);
        double** D = D_matrix(B);
        double** Tr = Tr_matrix();
        Wt = Wt_matrix(Wt, C, D, Tr);
        double** W = symetricm(Wt);

        HPEN BPen = CreatePen(PS_SOLID, 2, RGB(50, 0, 255));
        HPEN PPen = CreatePen(PS_SOLID, 2, RGB(153, 0, 153));
        HPEN RPen = CreatePen(PS_SOLID, 4, RGB(219, 56, 56));
        HPEN NoPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0));

        SelectObject(hdc, NoPen);
        Rectangle(hdc, 0, 0, 670, 700);

        //set parameters of the graph
        struct coords coords;
        double rad = 220;
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
        }

        //PRINT RANDOM MATRIX
        int startX_rm = 700;
        int startY_rm = 200;
        TextOut(hdc, startX_rm, startY_rm, L"Random matrix", 13);
        print_matrix(A, vertices, vertices, startX_rm, startY_rm, hdc);

        TextOut(hdc, startX_rm, startY_rm + 200, L"Weight", 6);
        print_matrix(W, vertices, vertices, startX_rm, startY_rm + 200, hdc);

        //DRAW GRAPH
        SetBkMode(hdc, TRANSPARENT);
        draw_undirectgraph(centerX, centerY, rad, vertex_rad, loop_rad, coords, A, W, PPen, hdc);

        //draw vertices
        SelectObject(hdc, BPen);
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(204, 204, 255));
        for (int i = 0; i < vertices; i++) {
            Ellipse(hdc, coords.nx[i] - vertex_rad, coords.ny[i] - vertex_rad, coords.nx[i] + vertex_rad, coords.ny[i] + vertex_rad);
            wchar_t buffer[5];
            swprintf(buffer, 5, L"%d", i + 1);
            TextOut(hdc, coords.nx[i] - dtx, coords.ny[i] - vertex_rad / 2, buffer, 2);
        }

        //MST
        struct Graph* graph = createGraph(A, W);
        struct Edge* mst = (struct Edge*)malloc(vertices * sizeof(struct Edge));
        find_mst(graph, mst, W);

        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(219, 150, 150));
        SelectObject(hdc, RPen);
        if (flag_tree == 1)
        {
            draw_mst(n_tree, mst, coords, vertex_rad, dtx, hdc);
        }

        EndPaint(hWnd, &ps);

        free_all(A, vertices);
        free_all(T, vertices);
        free_all(T2, vertices);
        free_all(Wt, vertices);
        free_all(W, vertices);
        free_all(B, vertices);
        free_all(C, vertices);
        free_all(D, vertices);
        free_all(Tr, vertices);
        free(mst);
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

void draw_undirectgraph(int centerX, int centerY, int rad, int vertex_rad, int loop_rad, struct coords coords, double** A, double** W,
    HPEN PPen, HDC hdc)
{
    int R = 12;
    int G = 7;
    int B = 3;
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            MoveToEx(hdc, coords.nx[i], coords.ny[i], NULL);
            if (A[i][j] == 1)
            {
                R += 10;
                G += 7;
                B += 15;
                HPEN ChangePen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
                SelectObject(hdc, ChangePen);
                if (i == j)
                {
                    //draw loops
                    SelectObject(hdc, PPen);
                    Ellipse(hdc, coords.loop_X[i] - loop_rad, coords.loop_Y[i] - loop_rad, coords.loop_X[i] + loop_rad, coords.loop_Y[i] + loop_rad);
                    SelectObject(hdc, ChangePen);
                }
                else
                {
                    LineTo(hdc, coords.nx[j], coords.ny[j]);

                    double x = fabs(coords.nx[i] + coords.nx[j]) / 2.;
                    double y = fabs(coords.ny[i] + coords.ny[j]) / 2.;
                    double text_align = 10;
                    double highlight_rad = 15;
                    SetDCBrushColor(hdc, RGB(R, G, B));
                    Ellipse(hdc, x - highlight_rad, y - highlight_rad, x + highlight_rad, y + highlight_rad);
                    wchar_t buffer[10];
                    swprintf(buffer, 5, L"%lf", W[i][j]);
                    TextOut(hdc, x - text_align, y - text_align, buffer, 3);
                }
            }
        }
    }
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
        for (int j = 0, x = startX; j < columns; j++, x += 40)
        {
            wchar_t buffer[10];
            swprintf(buffer, 5, L"%lf", matrix[i][j]);
            TextOut(hdc, x, y, buffer, 5);
        }
        MoveToEx(hdc, startX, y, NULL);
    }

}

void modify_matrix(double** T, double** A)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            double coef = 100 * A[i][j];
            T[i][j] *= coef;
        }
    }
}

double** roundm(double** T)
{
    double** Wt = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            Wt[i][j] = round(T[i][j]);
        }
    }
    return Wt;
}

double** B_matrix(double** Wt)
{
    double** B = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (Wt[i][j] == 0)
            {
                B[i][j] = 0;
            }
            else if(Wt[i][j] > 0)
            {
                B[i][j] = 1;
            }
        }
    }
    return B;
}

double** C_matrix(double** B)
{
    double** C = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (B[i][j] != B[j][i])
            {
                C[i][j] = 1;
            }
            else
            {
                C[i][j] = 0;
            }
        }
    }
    return C;
}

double** D_matrix(double** B)
{
    double** D = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (B[i][j] == B[j][i] && B[i][j] == 1)
            {
                D[i][j] = 1;
            }
            else
            {
                D[i][j] = 0;
            }
        }
    }
    return D;
}

double** Tr_matrix()
{
    double** Tr = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (j > i)
            {
                Tr[i][j] = 1;
            }
            else
            {
                Tr[i][j] = 0;
            }
        }
    }
    return Tr;
}

double** Wt_matrix(double** Wt, double** C, double** D, double** Tr)
{
    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            Wt[i][j] = Wt[i][j] * (C[i][j] + (D[i][j] * Tr[i][j]));
        }
    }
    return Wt;
}

double** symetricm(double** matrix2)
{
    double** matrix1 = init_double_matrix(vertices, vertices);
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            if (matrix2[i][j] != matrix2[j][i])
            {
                double value = (matrix2[i][j] == 0) ? matrix2[j][i] : matrix2[i][j];
                matrix1[i][j] = value;
                matrix1[j][i] = value;
            }
            else
            {
                matrix1[i][j] = matrix2[i][j];
            }
        }
    }
    return matrix1;
}

struct Graph* createGraph(double** matrix, double** W)
{
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));

    for (int i = 0; i < vertices; i++)
    {
        graph->head[i] = NULL;
    }

    for (int i = 0; i < vertices; i++)
    {
        for (int j = 0; j < vertices; j++)
        {
            if (matrix[i][j] == 1)
            {
                int src = i;
                int dest = j;
                double weight = W[i][j];

                struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
                new_node->dest = dest;
                new_node->weight = weight;
                new_node->next = graph->head[src];
                graph->head[src] = new_node;

                new_node = (struct Node*)malloc(sizeof(struct Node));
                new_node->dest = src;
                new_node->weight = weight;
                new_node->next = graph->head[dest];
                graph->head[dest] = new_node;
            }
        }
    }
    return graph;
}

int idx_of_min(int* weights, int* visited)
{
    int min = INT_MAX;
    int min_idx;

    for (int i = 0; i < vertices; i++)
    {
        if (visited[i] == 0 && weights[i] < min)
        {
            min = weights[i];
            min_idx = i;
        }
    }
    return min_idx;
}

void find_mst(struct Graph* graph, struct Edge* mst, double** W)
{
    int* sources = (int*)malloc(vertices * sizeof(int));
    int* weights = (int*)malloc(vertices * sizeof(int));
    int* visited = (int*)malloc(vertices * sizeof(int));

    for (int i = 0; i < vertices; i++)
    {
        sources[i] = 0;
        weights[i] = INT_MAX;
        visited[i] = 0;
    }

    weights[0] = 0;

    for (int i = 0; i < vertices; i++)
    {
        int min_idx = idx_of_min(weights, visited);
        visited[min_idx] = 1;

        print_visited(sources[min_idx], min_idx, W[sources[min_idx]][min_idx]);
        struct Edge* p_curr_mst = &mst[i];

        p_curr_mst->dest = min_idx;
        p_curr_mst->src = sources[min_idx];
        p_curr_mst->weight = W[sources[min_idx]][min_idx];


        struct Node* node = graph->head[min_idx];
        int dest;
        int weight;
        while (node != NULL)
        {
            dest = node->dest;
            weight = node->weight;

            if (visited[dest] == 0 && weight < weights[dest])
            {
                sources[dest] = min_idx;
                weights[dest] = weight;
            }
            node = node->next;
        }
    }
    free(sources);
    free(weights);
    free(visited);
}

void print_visited(int src, int dest, float weight)
{
    wchar_t buffer[32];
    swprintf(buffer, 32, L"%d - %d \t%lf \n", (src + 1), (dest + 1), weight);
    OutputDebugString(buffer);
}

void draw_mst(int n_tree, struct Edge* mst, struct coords coords, double vertex_rad, double dtx, HDC hdc)
{
    //draw lines, elipses for weights and weights
    for (int i = 0; i < n_tree; i++)
    {
        int src = mst[i].src;
        int dest = mst[i].dest;
        int weight = mst[i].weight;
        MoveToEx(hdc, coords.nx[src], coords.ny[src], NULL);
        LineTo(hdc, coords.nx[dest], coords.ny[dest]);

        double x = fabs(coords.nx[src] + coords.nx[dest]) / 2.;
        double y = fabs(coords.ny[src] + coords.ny[dest]) / 2.;
        double text_align = 10;
        double highlight_rad = 15;
        Ellipse(hdc, x - highlight_rad, y - highlight_rad, x + highlight_rad, y + highlight_rad);
        wchar_t buffer[10];
        swprintf(buffer, 5, L"%d", weight);
        TextOut(hdc, x - text_align, y - text_align, buffer, 3);
    }
    //draw vertices
    for (int i = 0; i < n_tree; i++)
    {
        SelectObject(hdc, GetStockObject(DC_BRUSH));
        SetDCBrushColor(hdc, RGB(219, 56, 56));
        int dest = mst[i].dest;
        Ellipse(hdc, coords.nx[dest] - vertex_rad, coords.ny[dest] - vertex_rad, coords.nx[dest] + vertex_rad, coords.ny[dest] + vertex_rad);
        wchar_t buffer2[5];
        swprintf(buffer2, 5, L"%d", dest + 1);
        TextOut(hdc, coords.nx[dest] - dtx, coords.ny[dest] - vertex_rad / 2, buffer2, 2);
    }
}