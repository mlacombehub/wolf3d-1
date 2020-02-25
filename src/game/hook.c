/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:32:16 by tbailleu          #+#    #+#             */
/*   Updated: 2020/02/25 16:37:32 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "engine.h"

uint32_t	start_timeout(uint32_t interval, t_engine *e)
{
	if (e->game.map.x == 0 && interval & 1)
	{
		write(2, "Timed-out: map read too slow\n", 29);
		write(2, "Press ^D to stop the program\n", 29);
	}
	if (e->quit & 0b001)
	{
		e->quit &= ~0b001;
		SDL_RemoveTimer(e->start_timeout);
		return (0);
	}
	return (interval & 1 ? 500 : 501);
}

uint32_t	tick_hook(uint32_t interval, t_engine *e)
{
	if (e->keys[SDL_SCANCODE_P] == 1)
	{
		e->keys[SDL_SCANCODE_P] = 43;
		if (++e->texture_random == 4)
			e->texture_random = 0;
	}
	if (e->keys[SDL_SCANCODE_O] == 1)
	{
		e->keys[SDL_SCANCODE_O] = 43;
		e->texture_random = 0;
		if (++e->image_random == 2)
			e->image_random = 0;
	}
	e->random = rand();
	player_movement(e, &e->game.map, &e->game.player);
	if (e->quit & 0b010)
	{
		e->quit &= ~0b010;
		SDL_RemoveTimer(e->tick_id);
		return (0);
	}
	return (interval);
}

uint32_t	render_hook(uint32_t interval, t_engine *e)
{
	SDL_SetRenderDrawColor(e->renderer, 41, 44, 57, 255);
	SDL_RenderClear(e->renderer);
	SDL_SetRenderDrawColor(e->renderer, 135, 206, 235, 255);
	SDL_RenderFillRect(e->renderer,
		&(SDL_Rect){0, 0, e->screen.w, e->screen.h / 2});
	render_raycast(e);
	if (e->game.map.x < 64 && e->game.map.y < 64)
		render_map(e);
	render_player(e);
	SDL_RenderPresent(e->renderer);
	if (e->quit & 0b100)
	{
		e->quit &= ~0b100;
		SDL_RemoveTimer(e->render_id);
		return (0);
	}
	return (interval);
}

int			events_hook(t_engine *e)
{
	SDL_Event	event;

	if (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || (event.type == SDL_KEYUP
		&& event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			return (e->quit = 0xffffffff);
		if (event.type == SDL_KEYDOWN)
			e->keys[event.key.keysym.scancode] = 1;
		else if (event.type == SDL_KEYUP)
			e->keys[event.key.keysym.scancode] = 0;
		if (event.type == SDL_MOUSEMOTION)
			e->mouse = (SDL_Point){event.motion.x, event.motion.y};
	}
	return (0);
}
