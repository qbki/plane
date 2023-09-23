#pragma once
#include "../scene.h"

void move_player(Scene::Meta& meta);
void rotate_player(Scene::Meta& meta);
void shoot_by_player(Scene::Meta& meta);
void handle_bullets (Scene::Meta& meta);
void handle_enemies_hunting(Scene::Meta& meta);
void handle_enemy_rotation(Scene::Meta& meta);
void handle_enemy_sinking(Scene::Meta& meta);
Scene::Handler make_cursor_handler(int& screen_width, int& screen_height);
