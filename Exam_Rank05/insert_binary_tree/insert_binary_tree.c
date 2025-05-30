#include <stdlib.h>
#include <stdio.h>
#include "ft_btree.h"

t_btree* insert_binary_tree(t_btree *root, int value)
{
	if (root == NULL)
	{
		t_btree *new_node = malloc(sizeof(t_btree));
		new_node->value = value;
		new_node->right = NULL;
		new_node->left = NULL;
		return (new_node);
	}
	else if(value < root->value)
		root->left = insert_binary_tree(root->left, value);
	else if(value > root->value)
		root->right = insert_binary_tree(root->right, value);
	return (root);
}

void print_in_order(t_btree *root)
{
	if (root == NULL)
		return;
	print_in_order(root->left);
	printf("%d ", root->value);
	print_in_order(root->right);
}

int main(void)
{
	t_btree *root = NULL;

	// Insert values into the tree
	int values[] = {50, 30, 70, 20, 40, 60, 80, 70}; // 70 is duplicated and should be ignored
	int n = sizeof(values) / sizeof(values[0]);

	for (int i = 0; i < n; i++)
		root = insert_binary_tree(root, values[i]);

	// Print the tree in-order (should be sorted)
	printf("In-order traversal: ");
	print_in_order(root);
	printf("\n");

	return 0;
}