/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 12:46:28 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/22 12:46:34 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	/* TODO : boucle du moniteur
	** while (is_running(sim)) {
	**   usleep(1000);   <- vérifie toutes les 1ms pour rester sous 10ms
	**   for each coder:
	**     if now - coder->last_compile_start > time_to_burnout:
	**       log "X burned out"
	**       sim_stop(sim)
	**       return NULL
	**   if all coders compiled >= nb_compiles_req:
	**       sim_stop(sim)
	**       return NULL
	** }
	*/
	(void)sim;
	return (NULL);
}
