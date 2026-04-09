/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:20:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/09 17:32:17 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"
#include <pthread.h>

void	ft_pq_swap(t_priority_q *q, t_coder *a, t_coder *b)
{
	t_coder	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

int	ft_pq_push(t_gen *g, t_priority_q *pq, t_coder *new)
{
	int	pos;

	pthread_mutex_lock(&pq->m_pq);
	if (pq->size >= pq->capacity)
		return (-1); //The list is full
	pos = pq->size;
	if (pos > 0 && pq->is_edf == 0)
	{
		pq->heap[pos] = *new;
		pq->size++;
	}
	else if (pos > 0 && pq->is_edf == 1)
	{
		pq->heap[pos] = *new;
		if (pq->heap[pos - 1].prior > pq->heap[pos].prior)
			ft_pq_swap(pq, &pq->heap[pos - 1], &pq->heap[pos]);
		pq->size++;
	}
	pthread_mutex_unlock(&pq->m_pq);
	return (0);
}

t_coder	ft_pq_pop(t_priority_q *pq)
{
	t_coder	returned;
	int	i;

	pthread_mutex_lock(&pq->m_pq);
	returned = pq->heap[0];
	i = -1;
	while (++i < pq->size - 1)
		pq->heap[i] = pq->heap[i + 1];
	pq->size--;
	pthread_mutex_unlock(&pq->m_pq);
	return (returned);
}
