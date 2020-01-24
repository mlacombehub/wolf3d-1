/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:41:24 by tbailleu          #+#    #+#             */
/*   Updated: 2019/12/20 18:22:10 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

void	render_player(t_engine *e)
{
	SDL_Rect		rect;
	double			dx;
	double			dy;
	int				s;

	s = e->screen.h / 4 / fmax(e->game.map.x, e->game.map.y);
	SDL_SetRenderDrawColor(e->renderer, 105, 79, 255, 255);
	rect = (SDL_Rect){s * e->game.player.x - 4, s * e->game.player.y - 4, 8, 8};
	SDL_RenderFillRect(e->renderer, &rect);
	SDL_SetRenderDrawColor(e->renderer, 255, 255, 255, 255);
	dx = s * e->game.player.x + sin(e->game.player.r - M_PI / 3) * s * 2;
	dy = s * e->game.player.y - cos(e->game.player.r - M_PI / 3) * s * 2;
	SDL_RenderDrawLine(e->renderer, s * e->game.player.x,
		s * e->game.player.y, dx, dy);
	dx = s * e->game.player.x + sin(e->game.player.r + M_PI / 3) * s * 2;
	dy = s * e->game.player.y - cos(e->game.player.r + M_PI / 3) * s * 2;
	SDL_RenderDrawLine(e->renderer, s * e->game.player.x,
		s * e->game.player.y, dx, dy);
}

void	render_map(t_engine *e)
{
	SDL_Rect	rect;
	int			x;
	int			y;
	int			scale;

	scale = e->screen.h / 4 / fmax(e->game.map.x, e->game.map.y);
	y = -1;
	while (++y < e->game.map.y)
	{
		x = -1;
		while (++x < e->game.map.x)
		{
			rect = (SDL_Rect){scale * x, scale * y, scale, scale};
			if (e->game.map.blocks[y][x] == ' ')
				SDL_SetRenderDrawColor(e->renderer, 90, 77, 65, 142);
			else
				SDL_SetRenderDrawColor(e->renderer, 255, 105, 79, 142);
			SDL_RenderFillRect(e->renderer, &rect);
		}
	}
}

void	player_movement(t_engine *e, t_map *map, t_player *p)
{
	double	speed;
	t_vec2d new;

	speed = e->keys[SDL_SCANCODE_LCTRL] ? .03 : .01;
	speed = e->keys[SDL_SCANCODE_RCTRL] ? .5 : speed;
	new = (t_vec2d){p->x, p->y};
	if (e->keys[SDL_SCANCODE_D] && !e->keys[SDL_SCANCODE_A])
		new = (t_vec2d){new.x + speed * cos(p->r), new.y + speed * sin(p->r)};
	if (e->keys[SDL_SCANCODE_A] && !e->keys[SDL_SCANCODE_D])
		new = (t_vec2d){new.x - speed * cos(p->r), new.y - speed * sin(p->r)};
	if (e->keys[SDL_SCANCODE_S] && !e->keys[SDL_SCANCODE_W])
		new = (t_vec2d){new.x - speed * sin(p->r), new.y + speed * cos(p->r)};
	if (e->keys[SDL_SCANCODE_W] && !e->keys[SDL_SCANCODE_S])
		new = (t_vec2d){new.x + speed * sin(p->r), new.y - speed * cos(p->r)};
	if (0 <= new.y && new.y < map->y && 0 <= p->x && p->x < map->x)
		if (map->blocks[(int)new.y][(int)p->x] == ' ')
			p->y = new.y;
	if (0 <= p->y && p->y < map->y && 0 <= new.x && new.x < map->x)
		if (map->blocks[(int)p->y][(int)new.x] == ' ')
			p->x = new.x;
	e->keys[SDL_SCANCODE_E] && !e->keys[SDL_SCANCODE_Q] ? p->r += .015 : 43;
	e->keys[SDL_SCANCODE_Q] && !e->keys[SDL_SCANCODE_E] ? p->r -= .015 : 43;
	p->r >= 2 * M_PI ? p->r -= 2 * M_PI : 43;
	p->r < 0 ? p->r += 2 * M_PI : 43;
}
