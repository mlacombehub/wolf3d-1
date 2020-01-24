/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:44:29 by tbailleu          #+#    #+#             */
/*   Updated: 2019/12/20 18:39:08 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "engine.h"

int			draw_text(t_engine *e, char *text, SDL_Point pos)
{
	SDL_Surface	*surface;
	SDL_Texture	*texture;
	SDL_Rect	rect;

	TTF_SizeText(e->font, text, &rect.w, &rect.h);
	rect.x = pos.x;
	rect.y = pos.y - rect.h;
	if (!(surface = TTF_RenderText_Solid(e->font, text,
		(SDL_Color){255, 255, 255, 255})))
		return (-1);
	if ((texture = SDL_CreateTextureFromSurface(e->renderer, surface)))
	{
		SDL_RenderCopy(e->renderer, texture, 0, &rect);
		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
		return (0);
	}
	SDL_FreeSurface(surface);
	return (-1);
}

void		draw_line_random_color(t_engine *e, SDL_Rect rect)
{
	int c;

	c = 0xffffff;
	if (e->texture_random == 2)
		c = e->random;
	else if (e->texture_random == 3)
		c = rand();
	c &= (int[4]){0xff6950, 0x4180db, 0x33c47f, 0xa061d1}[e->ray.orient];
	SDL_SetRenderDrawColor(e->renderer,
		c >> 16 & 0xff, c >> 8 & 0xff, c & 0xff, 255);
	SDL_RenderDrawLine(e->renderer, rect.x, rect.y, rect.w, rect.h);
}

void		draw_texture_raycast(t_engine *e, SDL_Point pos)
{
	int			index;
	SDL_Rect	orig;
	SDL_Rect	dest;

	dest = (SDL_Rect){pos.x, e->screen.h / 2 - pos.y, 1, pos.y * 2};
	if (e->image_random == 0)
	{
		orig = (SDL_Rect){e->textures_size[e->ray.orient].w *
			e->ray.tex_offset, 1, 1, e->textures_size[e->ray.orient].h};
		SDL_RenderCopy(e->renderer, e->textures[e->ray.orient], &orig, &dest);
	}
	else if (e->image_random == 1)
	{
		index = (int)(e->ray.pos.x + e->ray.pos.y + 42) % 2;
		orig = (SDL_Rect){e->students_size[index].w * e->ray.tex_offset, 1,
			1, e->students_size[index].h};
		SDL_RenderCopy(e->renderer, e->students[index], &orig, &dest);
	}
}
