/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 12:51:15 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/24 10:48:46 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	/* TODO : boucle principale du coder
	** while (is_running(sim)) {
	**   take_dongle(left), take_dongle(right)
	**   log "has taken a dongle" x2
	**   log "is compiling" + usleep(time_to_compile * 1000)
	**   release both dongles
	**   log "is debugging" + usleep(time_to_debug * 1000)
	**   log "is refactoring" + usleep(time_to_refactor * 1000)
	** }
	*/
	(void)coder;
	return (NULL);
}
