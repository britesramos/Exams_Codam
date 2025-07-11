#include <stdio.h>
#include <stdlib.h>

// Assuming ft_list.h looks like this:
typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void	ft_list_reverse(t_list **begin_list)
{
	t_list* tmp;
	t_list* tmp2;
	t_list* tmp3;
	tmp = *begin_list;
	if (!tmp || !tmp->next)
		return ;
	tmp2 = tmp->next;
	tmp3 = tmp->next->next;
	tmp->next = NULL;
	tmp2->next = tmp;
	while(tmp3){
		tmp = tmp2;
		tmp2 = tmp3;
		tmp3 = tmp3->next;
		tmp2->next = tmp;
	}
	*begin_list = tmp2;

}

void	ft_list_reverse2(t_list **begin_list)
{
	t_list *previous = NULL;
	t_list *current = *begin_list;
	t_list *next;
	if (!begin_list || next == NULL)
		return ;
	while(current != NULL)
	{
		next = current->next;
		current->next = previous;
		previous = current;
		current = next;
	}
	*begin_list = previous;
}

// Helper: create a new node
t_list *create_node(void *data)
{
	t_list *node = malloc(sizeof(t_list));
	if (!node)
		return NULL;
	node->data = data;
	node->next = NULL;
	return node;
}

// Helper: print the list
void print_list(t_list *head)
{
	while (head)
	{
		printf("%s -> ", (char *)head->data);
		head = head->next;
	}
	printf("NULL\n");
}

// Main to test the reverse function
int main(void)
{
	t_list *head = create_node("one");
	head->next = create_node("two");
	head->next->next = create_node("three");
	head->next->next->next = create_node("four");

	printf("Original list:\n");
	print_list(head);

	// ft_list_reverse(&head);
	ft_list_reverse2(&head);

	printf("\nReversed list:\n");
	print_list(head);

	// Freeing memory is recommended here
	return 0;
}
