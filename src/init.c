/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 10:57:37 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/16 10:57:40 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* ---------------------------------------------------------------- */
/*  Initialisation des dongles                                       */
/* ---------------------------------------------------------------- */

static int	init_one_dongle(t_dongle *d, int nb_coders)
{
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		pthread_mutex_destroy(&d->mutex);
		return (0);
	}
	d->available = 1;
	d->cooldown_until = 0;
	d->next_ticket = 0;
	if (!heap_init(&d->wait_queue, nb_coders))
	{
		pthread_cond_destroy(&d->cond);
		pthread_mutex_destroy(&d->mutex);
		return (0);
	}
	return (1);
}

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = malloc(sizeof(t_dongle) * sim->params.nb_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->params.nb_coders)
	{
		if (!init_one_dongle(&sim->dongles[i], sim->params.nb_coders))
		{
			while (--i >= 0)
			{
				pthread_cond_destroy(&sim->dongles[i].cond);
				pthread_mutex_destroy(&sim->dongles[i].mutex);
				heap_destroy(&sim->dongles[i].wait_queue);
			}
			free(sim->dongles);
			return (0);
		}
		i++;
	}
	return (1);
}

/* ---------------------------------------------------------------- */
/*  Initialisation des coders                                        */
/*                                                                   */
/*  Disposition circulaire :                                         */
/*  coder i -> dongle gauche = (i - 1 + N) % N                       */
/*             dongle droit  = i                                     */
/* ---------------------------------------------------------------- */

static void	init_coders(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->params.nb_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_count = 0;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].sim = sim;
		sim->coders[i].left_idx = (i - 1 + n) % n;
		sim->coders[i].right_idx = i;
		i++;
	}
}

/* ---------------------------------------------------------------- */
/*  Initialisation des deux mutex globaux (log + stop)               */
/* ---------------------------------------------------------------- */

static int	init_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	return (1);
}

/* ---------------------------------------------------------------- */
/*  init_sim                                                          */
/* ---------------------------------------------------------------- */

int	init_sim(t_sim *sim)
{
	sim->start_time = get_time_ms();
	sim->running = 1;
	sim->burnout_coder = -1;
	if (!init_mutexes(sim))
		return (0);
	sim->coders = malloc(sizeof(t_coder) * sim->params.nb_coders);
	if (!sim->coders)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	if (!init_dongles(sim))
	{
		free(sim->coders);
		pthread_mutex_destroy(&sim->stop_mutex);
		pthread_mutex_destroy(&sim->log_mutex);
		return (0);
	}
	init_coders(sim);
	return (1);
}
