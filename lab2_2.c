#include <stdio.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

typedef struct linked_list 
{
    char info;
    struct linked_list *prev_node;
    struct linked_list *next_node;
}l_list;

l_list *l_list_init(char item);
l_list *add_node(l_list *l_p, char item);
l_list *del_node(l_list *l_p);
char random_letter();

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
    unsigned int n;
    printf("Input number of items: ");
    scanf_s("%u", &n);
    char first_letter = random_letter();
    l_list *list_p = l_list_init(first_letter);
    l_list *first_node = list_p;
    for (unsigned int i = 0; i < n; i++)
    {
        char letter = random_letter();
        list_p = add_node(list_p, letter);
    }
    //TODO: sort
    while (list_p != NULL)
    {
        list_p = del_node(list_p);
    }
}

l_list *l_list_init(char item)
{
    l_list *l_p;
    l_p = malloc(sizeof(struct linked_list));
    *l_p = (l_list){
            .info = item,
            .prev_node = NULL,
            .next_node = NULL
           };
    printf("ptr=%d info =%c prev=%d next =%d\n", l_p, l_p->info, l_p->prev_node, l_p->next_node);
    return l_p;
}

l_list *add_node(l_list *l_p, char item)
{
    struct linked_list *p_node;
    p_node = malloc(sizeof(struct linked_list));
    p_node->info = item;
    p_node->prev_node = l_p;
    p_node->next_node = NULL;
    l_p->next_node = p_node;
    printf("ptr=%d info =%c prev=%d next =%d\n", p_node, p_node->info, p_node->prev_node, p_node->next_node);
    printf("ptr=%d info =%c prev=%d next =%d\n", l_p, l_p->info, l_p->prev_node, l_p->next_node);
    return p_node;
}

l_list *del_node(l_list *l_p)
{
    struct linked_list *p_node;
    p_node = l_p->prev_node;
    if (l_p != NULL)
    {
        free(l_p);
    }
    return p_node;
}

char random_letter()
{
    char letter;
    int len = 'z' - 'a' + 1;
    letter = 'a' + (rand() % len);
    return letter;
}