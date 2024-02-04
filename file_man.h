#ifndef FILE_MAN_H
#define FILE_MAN_H

#include <dirent.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> // Для работы со структурой stat
#include <limits.h> // Для PATH_MAX

// Прототипы функций
static void free_namelist(char ***namelist, int count);
void init_file_manager(const char *left_path, const char *right_path);
void display_directories();
void handle_input();
void cleanup_file_manager();

// Глобальные переменные для управления состоянием
static WINDOW *left_win, *right_win;
static char left_path[PATH_MAX], right_path[PATH_MAX];
static int current_side = 0; // 0 - левая панель, 1 - правая панель
static int left_index = 0, right_index = 0; // Текущий выбранный элемент в каждой панели
static int left_count = 0, right_count = 0; // Общее количество элементов в каждой панели
static char **left_namelist = NULL;
static char **right_namelist = NULL;

#endif
