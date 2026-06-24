/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 10:21:13 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/22 12:47:16 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	start_threads(t_sim *sim)
{
	int	i;

	if (pthread_create(&sim->monitor_thread, NULL, monitor_routine, sim) != 0)
		return (0);
	i = 0;
	while (i < sim->params.nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

static void	join_threads(t_sim *sim)
{
	int	i;

	pthread_join(sim->monitor_thread, NULL);
	i = 0;
	while (i < sim->params.nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_params	params;
	t_sim		sim;

	if (!parse_args(argc, argv, &params))
		return (1);
	sim.params = params;
	if (!init_sim(&sim))
	{
		fprintf(stderr, "Error: simulation initialization failed\n");
		return (1);
	}
	if (!start_threads(&sim))
	{
		fprintf(stderr, "Error: failed to create threads\n");
		cleanup_sim(&sim);
		return (1);
	}
	join_threads(&sim);
	cleanup_sim(&sim);
	return (0);
}
