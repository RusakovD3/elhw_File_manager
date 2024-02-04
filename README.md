# elhw_File_manager
## Запуск:
### gcc file_man.c -fPIC -c
### gcc -shared file_man.o -o libfileman.so
### gcc main.c libfileman.so -lncurses -o main -Wl,-rpath,.
### ./main 
