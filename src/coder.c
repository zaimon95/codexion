/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:10:56 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/24 10:11:03 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
** ANTI-DEADLOCK : on prend toujours le dongle de plus petit index
** en premier, quel que soit gauche/droite. Comme tous les coders
** suivent la meme regle, aucun cycle d'attente circulaire n'est
** possible (on casse une des 4 conditions de Coffman).
** Si le 2e dongle echoue (simulation arretee), on relache le 1er
** avant de sortir : jamais garder un dongle sans raison.
*/
static int	acquire_dongles(t_coder *coder)
{
	int	first;
	int	second;

	if (coder->left_idx < coder->right_idx)
	{
		first = coder->left_idx;
		second = coder->right_idx;
	}
	else
	{
		first = coder->right_idx;
		second = coder->left_idx;
	}
	if (!take_dongle(coder->sim, first, coder))
		return (0);
	if (!take_dongle(coder->sim, second, coder))
	{
		release_dongle(coder->sim, first);
		return (0);
	}
	return (1);
}

static void	release_dongles(t_coder *coder)
{
	release_dongle(coder->sim, coder->left_idx);
	release_dongle(coder->sim, coder->right_idx);
}

/*
** Une compilation complete : acquerir les 2 dongles, marquer le
** debut de compile (protege par state_mutex car le moniteur lit
** ce champ depuis un autre thread), dormir, relacher, incrementer
** le compteur. Retourne 0 si la simulation s'est arretee pendant
** l'acquisition (rien d'autre a faire, le coder doit sortir).
*/
static int	do_compile(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	if (!acquire_dongles(coder))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	log_state(sim, coder->id, "is compiling");
	usleep(sim->params.time_to_compile * 1000);
	release_dongles(coder);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	return (1);
}

static void	do_debug_refactor(t_coder *coder)
{
	t_sim	*sim;

	sim = coder->sim;
	log_state(sim, coder->id, "is debugging");
	usleep(sim->params.time_to_debug * 1000);
	log_state(sim, coder->id, "is refactoring");
	usleep(sim->params.time_to_refactor * 1000);
}

/*
** Boucle principale du coder : compile -> debug -> refactor -> repeat.
** Verifie is_running() a chaque etape pour s'arreter proprement
** des que le moniteur signale la fin (burnout ou objectif atteint).
*/
void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (is_running(coder->sim))
	{
		if (!do_compile(coder))
			break ;
		if (!is_running(coder->sim))
			break ;
		do_debug_refactor(coder);
	}
	return (NULL);
}
