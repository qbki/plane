#pragma once

#include "game_state.h"


void move_player(GameState::Meta& meta);
void rotate_player(GameState::Meta& meta);
void shoot_by_player(GameState::Meta& meta);
void handle_bullets (GameState::Meta& meta);
void handle_enemies_hunting(GameState::Meta& meta);
void handle_enemy_rotation(GameState::Meta& meta);
void handle_enemy_sinking(GameState::Meta& meta);
GameState::Handler make_cursor_handler(int& screen_width, int& screen_height);
