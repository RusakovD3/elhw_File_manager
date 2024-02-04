# elhw_File_manager
## Запуск:
#### sudo apt update
#### sudo apt-get install libncurses5-dev libncursesw5-dev
#### gcc file_man.c -fPIC -c
#### gcc -shared file_man.o -o libfileman.so
#### gcc main.c libfileman.so -lncurses -o main -Wl,-rpath,.
#### ./main 
