#include "file_man.h"
#include <stdio.h>
#include <stdlib.h> // Для getenv
#include <unistd.h> // Для getcwd
#include <limits.h> // Для PATH_MAX

int main() {
    atexit(cleanup_file_manager);

    char cwd[PATH_MAX];
    char *home_path;

    // Получение текущего рабочего каталога
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
        exit(EXIT_FAILURE);

    }

    // Получение пути домашнего каталога текущего пользователя
    home_path = getenv("HOME");
    if (home_path == NULL) {
        fprintf(stderr, "Unable to get home directory.\n");
        exit(EXIT_FAILURE);
    }

    // Инициализация файлового менеджера с текущим каталогом и домашним каталогом пользователя
    init_file_manager(home_path, cwd);

    // Обработка ввода пользователя
    handle_input();

    // Очистка перед выходом
    cleanup_file_manager();

    exit(EXIT_SUCCESS);
}
