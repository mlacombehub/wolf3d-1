/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:34:21 by tbailleu          #+#    #+#             */
/*   Updated: 2020/02/25 16:31:03 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "engine.h"

int		readfile(t_engine *e)
{
	char	buff[sizeof(t_game) + 1];
	size_t	i;
	int		res;

	i = 0;
	while (i <= sizeof(t_game))
	{
		if ((res = read(0, buff + i, sizeof(t_game) - i)) <= 0)
			break ;
		i += res;
	}
	if (res != 0 || i != sizeof(t_game))
		return (-1);
	ft_memcpy(&e->game, buff, sizeof(t_map));
	e->game.map.x = fmax(fmin(e->game.map.x, 255), 1);
	e->game.map.y = fmax(fmin(e->game.map.y, 255), 1);
	e->game.player.x = fmax(fmin(e->game.player.x, e->game.map.x - .5), .5);
	e->game.player.y = fmax(fmin(e->game.player.y, e->game.map.y - .5), .5);
	e->game.player.r = fmax(fmin(e->game.player.r, M_PI * 2), 0);
	return (0);
}

int		main(void)
{
	int			status;
	int			i;
	double		fov_width;
	t_engine	*e;
	SDL_Event	event;

	if (!(status = engine_init(&e)))
	{
		draw_text(e, MAP_TIMEOUT, (SDL_Point){0, 16});
		draw_text(e, STOP_TIMEOUT, (SDL_Point){0, 32});
		SDL_RenderPresent(e->renderer);
		SDL_PollEvent(&event);
		if (readfile(e))
			write(2, "Error: map invalid\n", 19);
		else
		{
			fov_width = tan(M_PI / 3) / (e->screen.w / 2);
			i = -1;
			while (++i < e->screen.w)
				e->angles[i] = atan((i - e->screen.w / 2) * fov_width);
			engine_loop(e, render_hook, tick_hook, events_hook);
		}
	}
	engine_quit(e);
	return (status);
}
