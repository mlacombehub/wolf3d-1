/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:47:28 by tbailleu          #+#    #+#             */
/*   Updated: 2019/12/21 11:26:38 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include "xpm/students.xpm"
#include "xpm/textures.xpm"

int		texture_init(t_engine *e)
{
	SDL_Surface	*surf;
	int			i;

	i = -1;
	while (++i < 4)
	{
		if (!(surf = IMG_ReadXPMFromArray(g_textures_xpm[i])))
			return (-8);
		if (!(e->textures[i] = SDL_CreateTextureFromSurface(e->renderer, surf)))
			return (-9);
		e->textures_size[i] = surf->clip_rect;
		SDL_FreeSurface(surf);
	}
	i = -1;
	while (++i < 2)
	{
		if (!(surf = IMG_ReadXPMFromArray(g_students_xpm[i])))
			return (-8);
		if (!(e->students[i] = SDL_CreateTextureFromSurface(e->renderer, surf)))
			return (-9);
		e->students_size[i] = surf->clip_rect;
		SDL_FreeSurface(surf);
	}
	return (0);
}

void	mutex_init(t_engine *e)
{
	int		i;

	i = -1;
	while (++i < 3)
	{
		e->lock_mutex[i] = SDL_CreateMutex();
		e->cond_signal[i] = SDL_CreateCond();
	}
}

int		engine_init(t_engine **e)
{
	if (!(*e = (t_engine*)malloc(sizeof(**e))))
		return (-1);
	ft_bzero(*e, sizeof(**e));
	if (SDL_Init(SDL_INIT_TIMER_VIDEO_EVENTS))
		return (-2);
	if (TTF_Init())
		return (-3);
	if (!((*e)->font = TTF_OpenFont(TTF_OPENFONT_PATH, 16)))
		return (-4);
	if (SDL_CreateWindowAndRenderer(0, 0,
		SDL_WINDOW_FULLSCREEN_DESKTOP, &(*e)->win, &(*e)->renderer))
		return (-5);
	if (SDL_GetDesktopDisplayMode(0, &(*e)->screen))
		return (-6);
	if (SDL_SetRenderDrawBlendMode((*e)->renderer, SDL_BLENDMODE_BLEND))
		return (-7);
	if (((*e)->random = texture_init(*e)))
		return ((*e)->random);
	mutex_init(*e);
	if (!((*e)->start_timeout = SDL_AddTimer(1000,
		(SDL_TimerCallback)start_timeout, *e)))
		return (-10);
	return (0);
}

void	engine_quit(t_engine *e)
{
	int		i;

	if (!e)
		return ;
	i = -1;
	while (++i < 4)
		SDL_DestroyTexture(e->textures[i]);
	i = -1;
	while (++i < 2)
		SDL_DestroyTexture(e->students[i]);
	if (e->renderer)
		SDL_DestroyRenderer(e->renderer);
	if (e->win)
		SDL_DestroyWindow(e->win);
	if (e->font)
		TTF_CloseFont(e->font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	free(e);
}

void	engine_loop(t_engine *e, t_timerhook render,
					t_timerhook tick, t_eventhook event)
{
	int	i;

	e->render_id = SDL_AddTimer(25, (SDL_TimerCallback)render, e);
	e->tick_id = SDL_AddTimer(5, (SDL_TimerCallback)tick, e);
	if (e->render_id && e->tick_id)
		while (43)
			if (event(e))
				break ;
	i = -1;
	while (++i < 3)
	{
		SDL_LockMutex(e->lock_mutex[i]);
		if (e->cond_signal[i])
			SDL_CondWait(e->cond_signal[i], e->lock_mutex[i]);
		SDL_UnlockMutex(e->lock_mutex[i]);
		SDL_DestroyCond(e->cond_signal[i]);
		SDL_DestroyMutex(e->lock_mutex[i]);
	}
}
