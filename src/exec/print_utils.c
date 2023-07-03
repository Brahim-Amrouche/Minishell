/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboulkh <maboulkh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 13:55:20 by maboulkh          #+#    #+#             */
/*   Updated: 2023/07/03 18:45:13 by maboulkh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void make_tree(t_exec_tree *tree, int depth, int offset, char **tree_d
//             , int *tree_size, char **types)
// {
// 	t_exec_info *node;

// 	if (tree->type == LOGICAL_EXEC)
// 	{
// 		node = tree->info.exec_node;
// 		if (depth > *tree_size)
// 			*tree_size = depth;
// 		tree_d[pow2(depth) + offset - 1] = node->content[0];
// 		if (!ft_strncmp("exit", node->content[0], -1))
// 			exit(0);
// 	}
// 	else
// 	{
// 		tree_d[pow2(depth) + offset - 1] = types[tree->type];
// 	}
// 	if (tree->left)
// 	{
// 		make_tree(tree->left, depth + 1, 2 * offset, tree_d, tree_size, types);
// 	}
// 	if (tree->right)
// 	{
// 		make_tree(tree->right, depth + 1, 2 * offset + 1
//             , tree_d, tree_size, types);
// 	}
// }

// void print_tree_n(char ** tree, int depth)
// {
// 	int i;
// 	int j;
// 	int size;
// 	int d;

// 	d = 0;
// 	size = 1;
// 	while (d < depth + 1)
// 	{
// 		if (d % 2)
// 			printf("    ");
// 		i = pow2(d) - 1;
// 		while (i < size)
// 		{
// 			j = 0;
// 			while (j < (pow2(depth) / pow2(d + 1)))
// 			{
// 				printf("        ");
// 				j++;
// 			}
// 			if (tree[i])
// 				printf("%8s,%d", tree[i], i);
// 			else
// 				printf("        ");
// 			i++;
// 		}
// 		printf("\n");
// 		d++;
// 		size = pow2(d + 1) - 1;dhjfdjskh
// 	}
// }

// int traverse_and_print_tree(t_exec_tree *tree)
// {
// 	char		**tree_d;
// 	char		*draw[200];
// 	int			tree_size;
// 	char		**types;

// 	tree_d = NULL;
// 	tree_size = 0;
// 	ft_memset(&draw, 0, sizeof(draw));
// 	types = ft_split("O | && || x", ' ');
// 	make_tree(tree, 0, 0, draw, &tree_size, types);
// 	print_tree_n(draw, tree_size);
// 	return (0);
// }

// int pow2(int n)
// {
// 	int res;

// 	res = 1;
// 	while (n--)
// 		res *= 2;
// 	return (res);
// }
