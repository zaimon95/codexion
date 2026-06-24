/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 12:48:45 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/24 10:54:29 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* ================================================================ */
/*  STUB — À implémenter dans heap.c                                */
/*  Ces fonctions seront la prochaine étape.                        */
/* ================================================================ */

int	heap_is_empty(t_heap *heap)
{
	return (heap->size == 0);
}

int	heap_insert(t_heap *heap, int coder_id, long long priority)
{
	(void)heap;
	(void)coder_id;
	(void)priority;
	return (1); // TODO
}

t_heap_node	heap_extract_min(t_heap *heap)
{
	t_heap_node	empty;

	(void)heap;
	empty.coder_id = -1;
	empty.priority = 0;
	return (empty); // TODO
}

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
