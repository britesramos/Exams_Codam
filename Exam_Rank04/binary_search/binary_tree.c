/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   binary_tree.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: sramos <sramos@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/01/10 14:31:44 by sramos        #+#    #+#                 */
/*   Updated: 2025/01/10 14:33:26 by sramos        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_tree
{
	int value;
	struct s_tree *left;
	struct s_tree *right;
} t_tree;

int	search_binary_tree(t_tree *root, int value)
{
	int	ret = 0;
	if (!root)
		return (0);
	if (root->value == value)
		return (1);
	ret = search_binary_tree(root->left, value);
	if (ret == 1)
		return (ret);
	ret = search_binary_tree(root->right, value);
	return (ret);
}