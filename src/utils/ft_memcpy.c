/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:43:56 by tbailleu          #+#    #+#             */
/*   Updated: 2019/12/20 15:44:04 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

void	*ft_memcpy(void *dst, void *src, size_t n)
{
	while (n > 7)
	{
		n -= 8;
		*((unsigned long long *)(dst + n)) = *((unsigned long long *)(src + n));
	}
	if (n > 3)
	{
		n -= 4;
		*((unsigned int *)(dst + n)) = *((unsigned int *)(src + n));
	}
	if (n > 1)
	{
		n -= 2;
		*((unsigned short int*)(dst + n)) = *((unsigned short int*)(src + n));
	}
	if (n > 0)
	{
		n -= 1;
		*((unsigned char *)(dst + n)) = *((unsigned char *)(src + n));
	}
	return (dst);
}
