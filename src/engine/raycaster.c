/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycaster.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:49:06 by tbailleu          #+#    #+#             */
/*   Updated: 2019/12/20 15:50:17 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"
#include <unistd.h>
#include <fcntl.h>

void	render_raycast(t_engine *e)
{
	int		i;
	double	height;

	i = -1;
	while (++i < e->screen.w)
	{
		rayon(e, e->angles[i] + e->game.player.r - M_PI / 2);
		height = e->screen.h / 1.6 / 3 / e->ray.dist / cos(e->angles[i]);
		if (e->texture_random)
			draw_line_random_color(e, (SDL_Rect){i, e->screen.h / 2 + height,
				i, e->screen.h / 2 - height});
		else
			draw_texture_raycast(e, (SDL_Point){i, height});
	}
}

void	dda_ray(t_game *game, t_ray *res, t_vec2d step, t_vec2d dist)
{
	t_vec2d		delta;

	delta = (t_vec2d){fabs(1 / res->tex_vec.x), fabs(1 / res->tex_vec.y)};
	while (res->block == 0)
	{
		if (dist.x < dist.y)
		{
			dist.x += delta.x;
			res->pos.x += step.x;
			res->orient = 0;
		}
		else
		{
			dist.y += delta.y;
			res->pos.y += step.y;
			res->orient = 1;
		}
		if (res->pos.y < 0 || game->map.y <= res->pos.y ||
			res->pos.x < 0 || game->map.x <= res->pos.x)
			res->block = -1;
		else if (game->map.blocks[(int)res->pos.y][(int)res->pos.x] != ' ')
			res->block = game->map.blocks[(int)res->pos.y][(int)res->pos.x];
	}
}

void	init_step_ray(t_game *game, t_ray *res, t_vec2d *step, t_vec2d *dist)
{
	t_vec2d		delta;

	delta = (t_vec2d){fabs(1 / res->tex_vec.x), fabs(1 / res->tex_vec.y)};
	step->x = res->tex_vec.x < 0 ? -1 : 1;
	if (res->tex_vec.x < 0)
		dist->x = (game->player.x - res->pos.x) * delta.x;
	else
		dist->x = (res->pos.x + 1.0 - game->player.x) * delta.x;
	step->y = res->tex_vec.y < 0 ? -1 : 1;
	if (res->tex_vec.y < 0)
		dist->y = (game->player.y - res->pos.y) * delta.y;
	else
		dist->y = (res->pos.y + 1.0 - game->player.y) * delta.y;
}

void	rayon(t_engine *e, double angle)
{
	t_vec2d		step;
	t_vec2d		dist;

	e->ray.tex_vec = (t_vec2d){cos(angle), sin(angle)};
	e->ray.pos = (t_vec2d){(int)e->game.player.x, (int)e->game.player.y};
	e->ray.block = 0;
	init_step_ray(&e->game, &e->ray, &step, &dist);
	dda_ray(&e->game, &e->ray, step, dist);
	if (e->ray.orient == 0)
	{
		e->ray.dist = fabs((e->ray.pos.x - e->game.player.x + (1 - step.x) / 2)
			/ e->ray.tex_vec.x);
		e->ray.tex_offset = e->game.player.y + e->ray.dist * e->ray.tex_vec.y;
	}
	else
	{
		e->ray.dist = fabs((e->ray.pos.y - e->game.player.y + (1 - step.y) / 2)
			/ e->ray.tex_vec.y);
		e->ray.tex_offset = e->game.player.x + e->ray.dist * e->ray.tex_vec.x;
	}
	if (e->ray.orient)
		e->ray.orient += e->ray.tex_vec.y < 0 ? 2 : 0;
	else
		e->ray.orient += e->ray.tex_vec.x < 0 ? 2 : 0;
	e->ray.tex_offset -= floor(e->ray.tex_offset);
}
