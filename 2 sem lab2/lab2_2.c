#include <stdio.h>
#include <stdlib.h>

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
l_list *sort(l_list *l_p);

int main()
{
    unsigned int n;
    printf("Input number of items: ");
    scanf_s("%u", &n);
    //create list
    char first_letter = random_letter();
    l_list *list_p = l_list_init(first_letter);
    l_list *first_node = list_p;
    for (unsigned int i = 0; i < n - 1; i++)
    {
        char letter = random_letter();
        list_p = add_node(list_p, letter);
    }
    list_p = first_node;
    //print unsorted
    printf("Unsorted list:\n");
    l_list *iterator = first_node;
    while(iterator != NULL)
    {
        printf("ptr=%d info =%c prev=%d next =%d\n", iterator, iterator->info, iterator->prev_node, iterator->next_node);
        iterator = iterator->next_node;
    }
    //sort
    list_p = sort(list_p);
    //print sorted
    printf("Sorted list:\n");
    while (list_p != NULL)
    {
        printf("ptr=%d info =%c prev=%d next =%d\n", list_p, list_p->info, list_p->prev_node, list_p->next_node);
        list_p = list_p->next_node;
    }
    //free
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
    return l_p;
}

l_list *add_node(l_list *l_p, char item)
{
    l_list *p_node;
    p_node = malloc(sizeof(struct linked_list));
    p_node->info = item;
    p_node->prev_node = l_p;
    p_node->next_node = NULL;
    l_p->next_node = p_node;
    return p_node;
}

l_list *del_node(l_list *l_p)
{
    l_list *p_node;
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

l_list *sort(l_list *list_p)
{
    l_list *first_node = list_p;
    l_list *current_node = first_node->next_node;
    while (current_node != NULL)
    {
        char current_info = current_node->info;
        l_list *current_next = current_node->next_node;
        l_list *current_prev = current_node->prev_node;
        l_list *p_node = first_node;
        
        while (p_node->info < current_info)
        {
            p_node = p_node->next_node;
        }
        if (p_node != current_node)
        {
            l_list *p_prev = p_node->prev_node;
            if (p_prev != NULL)
            {
                p_prev->next_node = current_node;
            }
            else
            {
                first_node = current_node;
            }
            p_node->prev_node = current_node;
            current_node->next_node = p_node;
            current_node->prev_node = p_prev;
            
            if (current_next != NULL)
            {
                current_next->prev_node = current_prev;
            }
            current_prev->next_node = current_next;
        }
        current_node = current_next;
    }
    return first_node;
}
