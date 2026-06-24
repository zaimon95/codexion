/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 10:37:15 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/24 10:51:49 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_int(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	validate_numeric_args(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!is_valid_int(argv[i]))
		{
			fprintf(stderr,
				"Error: argument '%s' must be a non-negative integer\n",
				argv[i]);
			return (0);
		}
		i++;
	}
	return (1);
}

void	fill_params(char **argv, t_params *p)
{
	p->nb_coders = atoi(argv[1]);
	p->time_to_burnout = atoi(argv[2]);
	p->time_to_compile = atoi(argv[3]);
	p->time_to_debug = atoi(argv[4]);
	p->time_to_refactor = atoi(argv[5]);
	p->nb_compiles_req = atoi(argv[6]);
	p->dongle_cooldown = atoi(argv[7]);
}
