/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 10:25:22 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/10 10:25:25 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_nodes(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

/*
** Remonte le dernier element insere tant que sa priorite est
** plus petite que celle de son parent (propriete du min-heap).
*/
void	heapify_up(t_heap *heap, int idx)
{
	int	parent;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (heap->nodes[idx].priority >= heap->nodes[parent].priority)
			break ;
		swap_nodes(&heap->nodes[idx], &heap->nodes[parent]);
		idx = parent;
	}
}

/*
** Trouve, parmi idx et ses deux enfants, celui qui a la plus
** petite priorite. Retourne son index.
*/
int	smallest_child(t_heap *heap, int idx)
{
	int	left;
	int	right;
	int	smallest;

	left = 2 * idx + 1;
	right = 2 * idx + 2;
	smallest = idx;
	if (left < heap->size
		&& heap->nodes[left].priority < heap->nodes[smallest].priority)
		smallest = left;
	if (right < heap->size
		&& heap->nodes[right].priority < heap->nodes[smallest].priority)
		smallest = right;
	return (smallest);
}

/*
** Fait descendre l'element a idx tant qu'un de ses enfants
** a une priorite plus petite que lui.
*/
void	heapify_down(t_heap *heap, int idx)
{
	int	smallest;

	smallest = smallest_child(heap, idx);
	while (smallest != idx)
	{
		swap_nodes(&heap->nodes[idx], &heap->nodes[smallest]);
		idx = smallest;
		smallest = smallest_child(heap, idx);
	}
}
