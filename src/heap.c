/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 10:25:02 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/10 10:25:04 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity)
{
	heap->nodes = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->nodes)
		return (0);
	heap->size = 0;
	heap->capacity = capacity;
	return (1);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->nodes);
	heap->nodes = NULL;
	heap->size = 0;
}

int	heap_is_empty(t_heap *heap)
{
	return (heap->size == 0);
}

int	heap_insert(t_heap *heap, int coder_id, long long priority)
{
	if (heap->size >= heap->capacity)
		return (0);
	heap->nodes[heap->size].coder_id = coder_id;
	heap->nodes[heap->size].priority = priority;
	heapify_up(heap, heap->size);
	heap->size++;
	return (1);
}

t_heap_node	heap_extract_min(t_heap *heap)
{
	t_heap_node	min;

	min = heap->nodes[0];
	heap->size--;
	heap->nodes[0] = heap->nodes[heap->size];
	if (heap->size > 0)
		heapify_down(heap, 0);
	return (min);
}
