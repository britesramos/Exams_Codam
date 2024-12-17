typedef struct s_tree
{
	struct t_tree_s* right;
	struct t_tree_s* left;
	int value;
} t_tree;

int search_binary_tree(t_tree* node, int value)
{
	if (node->left)
	{
		if (node->left.value == value)
		{
			return 1;
		}
		return (search_binary_tree(node->left, value));
	}
	if (node->right)
	{
		if (node->right.value == value)
		{
			return 1;
		}
		return (search_binary_tree(node->right, value));
	}
	return 0;
}

int main()
{
	t_tree *n1;
	t_tree *n2;
	t_tree *n3;
	t_tree *n4;
	t_tree *n5;
	t_tree *n6;
	t_tree *n7;
	t_tree *n8;
	t_tree *n9;
	t_tree *n10;
	t_tree *n11;

	n1->left = n2;
	n1->right = n3;
	n2->left = n4;
	n2->right = n5;
	n3->left = n6;
	n3->right = n7;
	n4->left = n9;
	n4->right = n10;
	n5->left = n11;
}