/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:56:14 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/16 10:56:16 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** Un coder s'inscrit dans la file d'attente du dongle (avec un ticket
** d'arrivee unique), puis attend son tour via wait_for_turn.
** Retourne 0 si la simulation s'est arretee pendant l'attente
** (le coder doit alors abandonner sans avoir pris le dongle).
*/
int	take_dongle(t_sim *sim, int dongle_idx, t_coder *coder)
{
	t_dongle	*d;
	long long	ticket;
	long long	priority;

	d = &sim->dongles[dongle_idx];
	pthread_mutex_lock(&d->mutex);
	ticket = d->next_ticket++;
	priority = compute_priority(sim, coder, ticket);
	heap_insert(&d->wait_queue, coder->id, priority);
	if (!wait_for_turn(sim, d, coder))
	{
		pthread_mutex_unlock(&d->mutex);
		return (0);
	}
	pthread_mutex_unlock(&d->mutex);
	log_state(sim, coder->id, "has taken a dongle");
	return (1);
}

/*
** Relache le dongle : il redevient disponible mais reste en
** cooldown pendant dongle_cooldown ms. On reveille tous les
** threads en attente ; celui dont c'est le tour reprendra la main
** (les autres se rendormiront en verifiant my_turn()).
*/
void	release_dongle(t_sim *sim, int dongle_idx)
{
	t_dongle	*d;

	d = &sim->dongles[dongle_idx];
	pthread_mutex_lock(&d->mutex);
	d->available = 1;
	d->cooldown_until = get_time_ms() + sim->params.dongle_cooldown;
	pthread_cond_broadcast(&d->cond);
	pthread_mutex_unlock(&d->mutex);
}
