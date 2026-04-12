/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:20:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/10 13:15:01 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"
#include <pthread.h>

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
	{
		pthread_mutex_unlock(&pq->m_pq);
		return (-1); //The list is full
	}
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
		if (pq->heap[pos - 1].prior > pq->heap[pos].prior)
			ft_pq_swap(&pq->heap[pos - 1], &pq->heap[pos]);
		pq->size++;
	}
	pthread_mutex_unlock(&pq->m_pq);
	return (0);
}

t_coder	*ft_pq_pop(t_priority_q *pq)
{
	t_coder	*returned;
	int	i;

	pthread_mutex_lock(&pq->m_pq);
	if (pq->size == 0)
		return (NULL);
	returned = pq->heap + 0;
	i = -1;
	while (++i < pq->size - 1)
		pq->heap[i] = pq->heap[i + 1];
	pq->size--;
	pthread_mutex_unlock(&pq->m_pq);
	return (returned);
}
