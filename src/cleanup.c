/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:11:58 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/24 10:12:00 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Detruit les "count" premiers dongles (mutex, cond, heap) et
** libere le tableau. Reutilisee par cleanup_sim ET par les
** chemins d'echec partiel d'init_sim.
*/
void	destroy_dongles(t_sim *sim, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		heap_destroy(&sim->dongles[i].wait_queue);
		i++;
	}
	free(sim->dongles);
}

/* Detruit les deux mutex globaux de la simulation. */
void	destroy_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->log_mutex);
}

void	cleanup_sim(t_sim *sim)
{
	int	i;

	destroy_dongles(sim, sim->params.nb_coders);
	i = 0;
	while (i < sim->params.nb_coders)
		pthread_mutex_destroy(&sim->coders[i++].state_mutex);
	free(sim->coders);
	destroy_mutexes(sim);
}
