/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinseo <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 21:57:26 by jinseo            #+#    #+#             */
/*   Updated: 2025/03/13 21:57:28 by jinseo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// 플레이어 이동 검증 함수
int is_move_valid(t_data *data, double new_x, double new_y)
{
    if (data->map.map[(int)new_y][(int)new_x] != '1' && 
        data->map.map[(int)new_y][(int)new_x] != 'X')
        return (1);
    return (0);
}

// 플레이어 위치 업데이트 함수
void update_player_position(t_data *data, double new_x, double new_y)
{
    data->map.map[(int)data->cor.y][(int)data->cor.x] = '2';
    data->cor.x = new_x;
    data->cor.y = new_y;
    data->map.map[(int)data->cor.y][(int)data->cor.x] = data->cor.c;
}

// W,A,S,D 키 처리 함수
void process_wasd_movement(int keycode, t_data *data)
{
    double angle;
    double new_x;
    double new_y;
    
    if (keycode == KEY_W || keycode == KEY_UP)
        angle = data->cor.dir;
    else if (keycode == KEY_S || keycode == KEY_DOWN)
        angle = data->cor.dir + M_PI;
    else if (keycode == KEY_A)
        angle = data->cor.dir - M_PI / 2;
    else if (keycode == KEY_D)
        angle = data->cor.dir + M_PI / 2;
    else
        return;
    
    new_x = data->cor.x + cos(angle) * 0.1;
    new_y = data->cor.y + sin(angle) * 0.1;
    
    if (is_move_valid(data, new_x, new_y))
        update_player_position(data, new_x, new_y);
}

// 화살표 키 처리 함수
void process_arrow_rotation(int keycode, t_data *data)
{
    if (keycode == KEY_LEFT)
    {
        data->cor.dir -= 0.1;
        if (data->cor.dir < 0)
            data->cor.dir += 2 * M_PI;
    }
    else if (keycode == KEY_RIGHT)
    {
        data->cor.dir += 0.1;
        if (data->cor.dir > 2 * M_PI)
            data->cor.dir -= 2 * M_PI;
    }
}

// 키 입력에 따른 게임 상태 업데이트 함수
void update_game_state(t_data *data)
{
    if (data->view_mode)
        render_3d(data);
}
