#!/bin/bash
gcc -Wall TD2.c -o AlignementOptimal.out
valgrind --leak-check=yes ./a.out texte1.txt texte2.txt
