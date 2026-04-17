/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:20:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/17 17:08:22 by joflorid         ###   ########.fr       */
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
	printf("Fin lista\n");
}

void	ft_pq_swap(t_coder *a, t_coder *b)
{
	t_coder	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	ft_pq_push(t_priority_q *pq, t_coder *new_coder)
{
	int	pos;

	pthread_mutex_lock(&pq->m_pq);
	if (pq->size >= pq->capacity)
		return (pthread_mutex_unlock(&pq->m_pq), -1); //The list is full
	pos = pq->size;
	if (pq->is_edf == 0)
	{
		pq->heap[pos] = *new_coder;
		pq->size++;
		pthread_mutex_unlock(&pq->m_pq);
		return (0);
	}
	else
	{
		pq->heap[pos] = *new_coder;
		pq->size++;
		if (pos > 0)
			if (pq->heap[pos - 1].prior > pq->heap[pos].prior
				|| (pq->heap[pos - 1].prior == pq->heap[pos].prior
					&& pq->heap[pos - 1].id < pq->heap[pos].id))
			ft_pq_swap(&pq->heap[pos - 1], &pq->heap[pos]);
	}
	pthread_mutex_unlock(&pq->m_pq);
	return (0);
}

void	ft_pq_pop(t_priority_q *pq, int coder_id)
{
	int	i;
	int	target;

	pthread_mutex_lock(&pq->m_pq);
	if (pq->size == 0)
	{
		pthread_mutex_unlock(&pq->m_pq);
		return ;
	}
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
	pthread_mutex_unlock(&pq->m_pq);
}

int	ft_pq_initial_push(t_priority_q *pq, t_coder *m)
{
	int	pos;

	pthread_mutex_lock(&pq->m_pq);
	if (pq->size >= pq->capacity)
		return (pthread_mutex_unlock(&pq->m_pq), -1); //The list is full
	pos = pq->size;
	pq->heap[pos] = *m;
	pq->size++;
	pthread_mutex_unlock(&pq->m_pq);
	return (0);
}
