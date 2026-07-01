/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/01 12:27:54 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/01 12:27:55 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/* Retourne le temps actuel en millisecondes */
long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000LL);
}

/* Retourne le temps écoulé depuis le début de la simulation (en ms) */
long long	get_elapsed_ms(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

/*
** Affiche un message de log : "TIMESTAMP CODER_ID msg\n"
** Le mutex garantit que deux messages ne se mélangent jamais.
*/
void	log_state(t_sim *sim, int coder_id, char *msg)
{
	long long	ts;

	ts = get_elapsed_ms(sim);
	pthread_mutex_lock(&sim->log_mutex);
	printf("%lld %d %s\n", ts, coder_id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}

/*
** Vérifie si la simulation est toujours en cours.
** Thread-safe : protégé par stop_mutex.
** Chaque thread doit appeler ça régulièrement pour savoir s'il doit s'arrêter.
*/
int	is_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->stop_mutex);
	running = sim->running;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (running);
}

/* Demande l'arrêt de la simulation (appelé par le moniteur) */
void	sim_stop(t_sim *sim)
{
	pthread_mutex_lock(&sim->stop_mutex);
	sim->running = 0;
	pthread_mutex_unlock(&sim->stop_mutex);
}
