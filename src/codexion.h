/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sla-gran <sla-gran@student.42belgium.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/24 10:11:40 by sla-gran          #+#    #+#             */
/*   Updated: 2026/07/24 10:11:41 by sla-gran         ###   ########.fr       */
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

/* ================================================================ */
/*  Scheduler                                                        */
/* ================================================================ */

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}	t_scheduler;

/* ================================================================ */
/*  Min-heap (file de priorité) — un nœud = un coder en attente     */
/* ================================================================ */

typedef struct s_heap_node
{
	int			coder_id;
	long long	priority;	/* FIFO: timestamp arrivée | EDF: deadline */
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*nodes;
	int			size;
	int			capacity;
}	t_heap;

/* ================================================================ */
/*  Paramètres de simulation (argv parsé une fois, puis read-only)  */
/* ================================================================ */

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

/* ================================================================ */
/*  Forward declaration (t_coder référence t_sim, et vice versa)    */
/* ================================================================ */

typedef struct s_sim	t_sim;

/* ================================================================ */
/*  Dongle USB                                                       */
/* ================================================================ */

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;		/* 1 = libre, 0 = pris */
	long long		cooldown_until;	/* timestamp (ms) fin du cooldown */
	long long		next_ticket;	/* compteur de tickets d'arrivee */
	t_heap			wait_queue;		/* coders en attente (heap) */
}	t_dongle;

/* ================================================================ */
/*  Coder = un thread POSIX                                         */
/* ================================================================ */

typedef struct s_coder
{
	int				id;
	int				compile_count;
	long long		last_compile_start;	/* pour burnout + deadline EDF */
	pthread_mutex_t	state_mutex;		/* protege les 2 champs ci-dessus */
	pthread_t		thread;
	t_sim			*sim;				/* accès à la simulation */
	int				left_idx;			/* index du dongle gauche */
	int				right_idx;			/* index du dongle droit */
}	t_coder;

/* ================================================================ */
/*  Simulation — état global passé PAR POINTEUR, jamais global var  */
/* ================================================================ */

struct s_sim
{
	t_params		params;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start_time;		/* timestamp (ms) début simulation */
	pthread_mutex_t	log_mutex;		/* sérialiser les printf */
	pthread_mutex_t	stop_mutex;		/* protéger running + burnout_coder */
	int				running;		/* 1 = en cours | 0 = arrêt demandé */
	int				burnout_coder;	/* id du coder brûlé (-1 si aucun) */
	pthread_t		monitor_thread;
};

/* ================================================================ */
/*  Prototypes — organisés par fichier source                       */
/* ================================================================ */

/* utils.c */
long long	get_time_ms(void);
long long	get_elapsed_ms(t_sim *sim);
void		log_state(t_sim *sim, int coder_id, char *msg);
int			is_running(t_sim *sim);
void		sim_stop(t_sim *sim);

/* args.c */
int			parse_args(int argc, char **argv, t_params *params);

/* init.c */
int			init_sim(t_sim *sim);

/* cleanup.c */
void		cleanup_sim(t_sim *sim);
void		destroy_dongles(t_sim *sim, int count);
void		destroy_mutexes(t_sim *sim);

/* heap.c */
int			heap_init(t_heap *heap, int capacity);
void		heap_destroy(t_heap *heap);
int			heap_insert(t_heap *heap, int coder_id, long long priority);
t_heap_node	heap_extract_min(t_heap *heap);
int			heap_is_empty(t_heap *heap);

/* heap_utils.c */
void		swap_nodes(t_heap_node *a, t_heap_node *b);
void		heapify_up(t_heap *heap, int idx);
void		heapify_down(t_heap *heap, int idx);
int			smallest_child(t_heap *heap, int idx);
t_heap_node	heap_peek(t_heap *heap);

/* dongle.c */
int			take_dongle(t_sim *sim, int dongle_idx, t_coder *coder);
void		release_dongle(t_sim *sim, int dongle_idx);

/* dongle_utils.c */
long long	compute_priority(t_sim *sim, t_coder *coder, long long ticket);
int			wait_for_turn(t_sim *sim, t_dongle *d, t_coder *coder);

/* coder.c */
void		*coder_routine(void *arg);

/* monitor.c */
void		*monitor_routine(void *arg);

#endifendif
