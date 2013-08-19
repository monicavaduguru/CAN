all:    list.h stringtocart.h parsing.h main_file.c
        gcc -o  P1 main_file.c -lm
debug:  list.h stringtocart.h parsing.h main_file.c
        gcc -g -o P1 main_file.c
clean:
        rm -f *.o *~ P1 core
