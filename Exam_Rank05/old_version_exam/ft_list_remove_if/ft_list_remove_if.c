#include "../ft_list.h"
#include <stdlib.h>

void		ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	t_list *previous = NULL;
	t_list *current = begin_list;
	t_list *next;

	if(!begin_list || !*begin_list)
		return ;
	while (current) //while current exist. That would iterate trought the list.
	{
		next = current->next; //Initialize next. To current->next.
		if ((*cmp)(current->data, data_ref) == 0) //If the current node matches the condition to be removed.
		{
			if (previous == NULL) //If this is the begining of the list.
			{
				*begin_list = next; //Move the list begin pointer to the next.
			}
			else
			{
				previous->next = next; //Else just move the previous point to be next node.
			}
			free(current); //Free the current node. As is the pointer to be removed.
		}
		else
			previous = current; //If everything is fine, move every pointer(previous, current and next).
		current = next;

	}
}