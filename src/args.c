/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/22 12:51:35 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/24 10:37:52 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	validate_bounds(t_params *p)
{
	if (p->nb_coders < 1)
	{
		fprintf(stderr, "Error: nb_coders must be >= 1\n");
		return (0);
	}
	if (p->time_to_burnout < 1)
	{
		fprintf(stderr, "Error: time_to_burnout must be >= 1\n");
		return (0);
	}
	return (1);
}

static int	validate_scheduler(char *arg, t_params *p)
{
	if (strcmp(arg, "fifo") == 0)
		p->scheduler = SCHEDULER_FIFO;
	else if (strcmp(arg, "edf") == 0)
		p->scheduler = SCHEDULER_EDF;
	else
	{
		fprintf(stderr, "Error: scheduler must be 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

int	parse_args(int argc, char **argv, t_params *params)
{
	if (argc != 9)
	{
		fprintf(stderr,
			"Usage: ./codexion nb_coders time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor nb_compiles_req "
			"dongle_cooldown [fifo|edf]\n");
		return (0);
	}
	if (!validate_numeric_args(argv))
		return (0);
	fill_params(argv, params);
	if (!validate_bounds(params))
		return (0);
	return (validate_scheduler(argv[8], params));
}
