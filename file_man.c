#include "file_man.h"

void init_file_manager(const char *left_init_path, const char *right_init_path) {
    // Инициализация ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Создание окон для левой и правой панели
    int height, width;
    getmaxyx(stdscr, height, width);
    left_win = newwin(height, width / 2, 0, 0);
    right_win = newwin(height, width / 2, 0, width / 2);

    // Инициализация путей
    strncpy(left_path, left_init_path, PATH_MAX);
    strncpy(right_path, right_init_path, PATH_MAX);

    // Отображение начального состояния
    display_directories();
}

void display_directory_contents(WINDOW *win, const char *path, int *count, char ***namelist) {
    // Очистка окна и установка начальной позиции
    werase(win);
    int line = 0;

    struct dirent **temp_namelist;
    int n = scandir(path, &temp_namelist, NULL, alphasort);

    // Освобождаем предыдущий список имен, если он существует
    free_namelist(namelist, *count);
    *count = n; // Обновляем общее количество элементов для текущей панели

    // Выделяем память для нового списка имен
    *namelist = (char **)malloc(n * sizeof(char *));

    if (n < 0) {
        // Ошибка при чтении директории
        wprintw(win, "Failed to read directory: %s\n", path);
    } else {
        // Перебор файлов и директорий
        for (int i = 0; i < n; i++) {
            // Вывод имени файла или директории
            mvwprintw(win, line++, 1, "%s", temp_namelist[i]->d_name);
            // Сохраняем имя в глобальный список
            (*namelist)[i] = strdup(temp_namelist[i]->d_name);
            free(temp_namelist[i]);
        }
        free(temp_namelist);
    }

    // Обновление окна
    box(win, 0, 0);
    wrefresh(win);
}

void display_directories() {
    display_directory_contents(left_win, left_path, &left_count, &left_namelist);
    display_directory_contents(right_win, right_path, &right_count, &right_namelist);
    // Перемещаем курсор на выбранную директорию в активном окне
    if (current_side == 0) {
        wmove(left_win, left_index, 1);
        wrefresh(left_win);
    } else {
        wmove(right_win, right_index, 1);
        wrefresh(right_win);
    }

}

void enter_directory() {
    char **current_namelist = current_side == 0 ? left_namelist : right_namelist;
    char *selected_name = current_namelist[current_side == 0 ? left_index : right_index];
    char *current_path = current_side == 0 ? left_path : right_path;
    char new_path[PATH_MAX];
    // Строим новый путь
    snprintf(new_path, PATH_MAX, "%s/%s", current_path, selected_name);

    // Проверяем, является ли выбранный элемент директорией
    struct stat statbuf;
    if (stat(new_path, &statbuf) == 0) {
        if (S_ISDIR(statbuf.st_mode)) {
            // Обновляем текущий путь и перезагружаем содержимое
            strncpy(current_path, new_path, PATH_MAX);
            if (current_side == 0) {
                left_index = 0; // Сброс индекса для левой панели
            } else {
                right_index = 0; // Сброс индекса для правой панели
            }
            display_directories(); // Обновление отображения директорий
        }
    }
}

void handle_input() {
    int ch;
    // Включаем обработку специальных клавиш для окон
    keypad(left_win, TRUE);
    keypad(right_win, TRUE);
    
    while (1) {
        // Получаем ввод с активного окна
        ch = wgetch(current_side == 0 ? left_win : right_win);

        if (ch == 'q') { // Выход из цикла при нажатии 'q'
            break;
        }

        switch (ch) {
            case KEY_UP:
                if (current_side == 0 && left_index > 0) left_index--;
                else if (current_side == 1 && right_index > 0) right_index--;
                break;
            case KEY_DOWN:
                if (current_side == 0 && left_index < left_count - 1) left_index++;
                else if (current_side == 1 && right_index < right_count - 1) right_index++;
                break;
            case '\t': // Переключение между панелями
                current_side = !current_side;
                break;
            case '\n': // Правильная обработка Enter для перехода в директорию
                enter_directory();
                break;
        }
        display_directories(); // Обновляем директории после каждого действия
    }
}

void cleanup_file_manager() {
    // Освобождаем память, выделенную для списков имен
    free_namelist(&left_namelist, left_count);
    free_namelist(&right_namelist, right_count);

    // Очистка и выход из ncurses
    delwin(left_win);
    delwin(right_win);
    endwin();
}

// Функция для освобождения памяти, выделенной под список имен
static void free_namelist(char ***namelist, int count) {
    if (*namelist != NULL) {
        for (int i = 0; i < count; i++) {
            free((*namelist)[i]);
        }
        free(*namelist);
        *namelist = NULL;
    }
}
