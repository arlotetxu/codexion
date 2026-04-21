/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:20:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/21 17:17:21 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <pthread.h>
#include <stdio.h>

void	ft_print_heaps(t_gen *g)
{
	int	i;
	int	j;

	i = 0;
	while (i < g->p->num_coders)
	{
		pthread_mutex_lock(&g->m_print);
		printf("\nDongle right de %i:\n", g->c[i].id);
		j = 0;
		while (g->c[i].right->pq->heap[j].id)
		{
			printf("[%i] Coder ID: %i\n", j, g->c[i].right->pq->heap[j].id);
			j++;
		}
		printf("Dongle left de %i:\n", g->c[i].id);
		j = 0;
		while (g->c[i].left->pq->heap[j].id)
		{
			printf("[%i]Coder ID: %i\n", j, g->c[i].left->pq->heap[j].id);
			j++;
		}
		pthread_mutex_unlock(&g->m_print);
		i++;
	}
}

void	ft_pq_swap(t_coder *a, t_coder *b)
{
	t_coder	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	ft_pq_push(t_priority_q *pq, t_coder *m)
{
	int	pos;

	if (pq->size >= pq->capacity)
		return (-1);
	pos = pq->size;
	if (pq->is_edf == 0)
		pq->heap[pos] = *m;
	else
	{
		pq->heap[pos] = *m;
		if (pos > 0)
		{
			// printf("\e[0;31mprior pos-1[%i]: %li / prior pos[%i]: %li\n\e[0m",
			// 	pq->heap[pos - 1].id, pq->heap[pos - 1].prior, pq->heap[pos].id,
			// 	pq->heap[pos].prior);
			if (pq->heap[pos - 1].prior > pq->heap[pos].prior
				|| (pq->heap[pos - 1].prior == pq->heap[pos].prior
					&& pq->heap[pos - 1].id < pq->heap[pos].id))
				ft_pq_swap(&pq->heap[pos - 1], &pq->heap[pos]);
		}
	}
	pq->size++;
	return (0);
}

void	ft_pq_pop(t_priority_q *pq, int coder_id)
{
	int	i;
	int	target;

	if (pq->size == 0)
		return ;
	target = -1;
	i = -1;
	while (++i < pq->size)
		if (pq->heap[i].id == coder_id)
			target = i;
	if (target != -1)
	{
		while (target < pq->size - 1)
		{
			pq->heap[target] = pq->heap[target + 1];
			target++;
		}
		pq->size--;
	}
}

int	ft_pq_initial_push(t_priority_q *pq, t_coder *m)
{
	int	pos;

	if (pq->size >= pq->capacity)
		return (-1);
	pos = pq->size;
	pq->heap[pos] = *m;
	pq->size++;
	return (0);
}
