/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/08 12:20:53 by joflorid          #+#    #+#             */
/*   Updated: 2026/04/24 13:40:07 by joflorid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/codexion.h"

void	*ft_add_to_pq(t_coder *m)
{
	pthread_mutex_lock(&m->left->m_dongle);
	ft_pq_push(m->left->pq, m);
	pthread_mutex_unlock(&m->left->m_dongle);
	if (m->right != NULL)
	{
		pthread_mutex_lock(&m->right->m_dongle);
		ft_pq_push(m->right->pq, m);
		pthread_mutex_unlock(&m->right->m_dongle);
	}
	return (NULL);
}

void	ft_pq_swap(t_coder **a, t_coder **b)
{
	t_coder	*temp;

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
		pq->heap[pos] = m;
	else
	{
		pq->heap[pos] = m;
		if (pos > 0)
		{
			if (pq->heap[pos - 1]->prior > pq->heap[pos]->prior
				|| (pq->heap[pos - 1]->prior == pq->heap[pos]->prior
					&& pq->heap[pos - 1]->id < pq->heap[pos]->id))
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
		if (pq->heap[i]->id == coder_id)
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
	pq->heap[pos] = m;
	pq->size++;
	return (0);
}
