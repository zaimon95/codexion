/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 14:17:06 by sla-gran          #+#    #+#             */
/*   Updated: 2026/06/24 10:51:40 by sla-gran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

typedef struct s_heap_node
{
	int			coder_id;
	long long	priority;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_params
{
	int			nb_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			nb_compiles_req;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_params;

typedef struct s_sim	t_sim;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	long long		cooldown_until;
	t_heap			wait_queue;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	long long	last_compile_start;
	pthread_t	thread;
	t_sim		*sim;
	int			left_idx;
	int			right_idx;
}	t_coder;

struct s_sim
{
	t_params		params;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start_time;
	pthread_mutex_t	log_mutex;
	pthread_mutex_t	stop_mutex;
	int				running;
	int				burnout_coder;
	pthread_t		monitor_thread;
};

/* utils.c */
long long	get_time_ms(void);
long long	get_elapsed_ms(t_sim *sim);
void		log_state(t_sim *sim, int coder_id, char *msg);
int			is_running(t_sim *sim);
void		sim_stop(t_sim *sim);

/* args.c */
int			parse_args(int argc, char **argv, t_params *params);

/* args_utils.c */
int			is_valid_int(char *str);
int			validate_numeric_args(char **argv);
void		fill_params(char **argv, t_params *p);

/* init.c */
int			init_sim(t_sim *sim);
void		cleanup_sim(t_sim *sim);

/* heap.c */
int			heap_init(t_heap *heap, int capacity);
void		heap_destroy(t_heap *heap);
int			heap_insert(t_heap *heap, int coder_id, long long priority);
t_heap_node	heap_extract_min(t_heap *heap);
int			heap_is_empty(t_heap *heap);

/* dongle.c */
void		take_dongle(t_sim *sim, int dongle_idx, t_coder *coder);
void		release_dongle(t_sim *sim, int dongle_idx);

/* coder.c */
void		*coder_routine(void *arg);

/* monitor.c */
void		*monitor_routine(void *arg);

#endif
