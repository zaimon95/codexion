/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:56:54 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/16 10:56:56 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** FIFO : la priorite EST le ticket d'arrivee -> ordre strict d'arrivee.
** EDF  : la priorite est la deadline de burnout, avec le ticket encode
** dans les bits de poids faible pour departager les egalites de
** deadline de facon deterministe (exige par le sujet).
*/
long long	compute_priority(t_sim *sim, t_coder *coder, long long ticket)
{
	long long	deadline;

	if (sim->params.scheduler == SCHEDULER_EDF)
	{
		deadline = coder->last_compile_start + sim->params.time_to_burnout;
		return ((deadline << 20) | (ticket & 0xFFFFF));
	}
	return (ticket);
}

static void	ms_to_timespec(long long ms, struct timespec *ts)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000;
}

/*
** Vrai si c'est au tour de "coder" de prendre le dongle :
** le dongle doit etre libre, hors cooldown, et coder doit etre
** en tete de la file d'attente (heap_peek sans le retirer).
*/
static int	my_turn(t_dongle *d, t_coder *coder)
{
	t_heap_node	top;

	if (!d->available || get_time_ms() < d->cooldown_until)
		return (0);
	top = heap_peek(&d->wait_queue);
	return (top.coder_id == coder->id);
}

/*
** Boucle d'attente. Si le dongle est en cooldown, on utilise
** pthread_cond_timedwait avec pour echeance la fin du cooldown :
** ca reveille le thread meme si personne n'appelle broadcast a
** ce moment precis. Sinon, cond_wait classique (reveille par
** release_dongle). On sort aussi si la simulation s'arrete.
*/
int	wait_for_turn(t_sim *sim, t_dongle *d, t_coder *coder)
{
	struct timespec	ts;

	while (is_running(sim) && !my_turn(d, coder))
	{
		if (d->available && get_time_ms() < d->cooldown_until)
		{
			ms_to_timespec(d->cooldown_until, &ts);
			pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
		}
		else
			pthread_cond_wait(&d->cond, &d->mutex);
	}
	if (!is_running(sim))
		return (0);
	heap_extract_min(&d->wait_queue);
	d->available = 0;
	return (1);
}
