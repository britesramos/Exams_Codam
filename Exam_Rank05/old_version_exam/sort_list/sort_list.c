#include "../ft_list.h"
#include <stdlib.h>

t_list *sort_list(t_list *lst, int (*cmp)(int, int))
{
	t_list *start = lst; //To keep a pointer to the begiin of the linked list.
	int temp = 0; //To store data momentarily

	if (!lst)
		return (NULL);
	while(lst && lst->next) //To make sure we compare 2 nodes, and not with a NULL pointer.
	{
		if ((*cmp)(lst->data, lst->next->data) == 0) //If cmp is true it means that we need to swap values.
		{
			temp = lst->data;
			lst->data = lst->next->data;
			lst->next->data = temp;
			lst = start; //Reset lst, so the loop goes back to the begining, until the linked list is completely sorted.
		}
		else
			lst = lst->next;
	}
	return (start);
}