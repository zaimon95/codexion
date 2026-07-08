/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 12:51:27 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/03 14:01:21 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_sim *sim, int dongle_idx, t_coder *coder)
{
	(void)sim;
	(void)dongle_idx;
	(void)coder;
	sim->
	/* TODO : lock le mutex du dongle, attendre qu'il soit libre,
	** respecter le cooldown, utiliser le scheduler (FIFO/EDF) */
}

void	release_dongle(t_sim *sim, int dongle_idx)
{
	(void)sim;
	(void)dongle_idx;
	/* TODO : marquer le dongle disponible, noter cooldown_until,
	** réveiller le prochain coder dans la wait_queue */
}
