/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbailleu <tbailleu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/20 15:25:46 by tbailleu          #+#    #+#             */
/*   Updated: 2020/02/25 16:31:00 by tbailleu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_H
# define ENGINE_H

# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_image.h>

# define SDL_INIT_TIMER_VIDEO_EVENTS 0x00004021
# define TTF_OPENFONT_PATH "/System/Library/Fonts/menlo.ttc"
# define MAP_TIMEOUT "Timed-out: read map from stdin is too slow"
# define STOP_TIMEOUT "Press ^D in iTerm to stop the map reader"

typedef	char		t_block;

typedef struct		s_vec2d
{
	double	x;
	double	y;
}					t_vec2d;

typedef struct		s_map
{
	int		x;
	int		y;
	t_block	blocks[256][257];
}					t_map;

typedef struct		s_player
{
	double	x;
	double	y;
	double	r;
}					t_player;

typedef	struct		s_game
{
	t_player		player;
	t_map			map;
}					t_game;

typedef struct		s_ray
{
	t_block			block;
	t_vec2d			pos;
	double			dist;
	int				orient;
	double			tex_offset;
	t_vec2d			tex_vec;
}					t_ray;

typedef struct		s_engine
{
	int				keys[SDL_NUM_SCANCODES];
	int				random;
	int				quit;
	SDL_TimerID		tick_id;
	SDL_TimerID		render_id;
	SDL_TimerID		start_timeout;
	SDL_Point		mouse;
	SDL_DisplayMode	screen;
	TTF_Font		*font;
	SDL_Window		*win;
	SDL_Renderer	*renderer;
	SDL_Texture		*students[2];
	SDL_Rect		students_size[2];
	SDL_Texture		*textures[4];
	SDL_Rect		textures_size[4];
	double			angles[5120];
	int				texture_random;
	int				image_random;
	t_ray			ray;
	t_game			game;
}					t_engine;

typedef uint32_t	(*t_timerhook)(uint32_t interval, t_engine *param);
typedef int			(*t_eventhook)(t_engine *param);

void				ft_bzero(void *s, size_t n);
void				*ft_memcpy(void *dst, void *src, size_t n);

int					draw_text(t_engine *e, char *text, SDL_Point pos);
void				draw_line_random_color(t_engine *e, SDL_Rect rect);
void				draw_texture_raycast(t_engine *e, SDL_Point pos);

void				rayon(t_engine *e, double angle);

uint32_t			tick_hook(uint32_t interval, t_engine *e);
uint32_t			clock_hook(uint32_t interval, t_engine *e);
uint32_t			render_hook(uint32_t interval, t_engine *e);
uint32_t			start_timeout(uint32_t interval, t_engine *e);
int					events_hook(t_engine *e);

void				render_raycast(t_engine *e);
void				render_player(t_engine *e);
void				render_map(t_engine *e);

void				player_movement(t_engine *e, t_map *map, t_player *p);

int					engine_init(t_engine **e);
void				engine_loop(t_engine *e, t_timerhook render,
						t_timerhook tick, t_eventhook event);
void				engine_quit(t_engine *e);

#endif
