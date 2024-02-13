#include <stdio.h>
#include <stdlib.h>

// int var_data = 5;
// const int var_rodata = 10;
// int var_bss;

void function2() {
    int var_pile4;
    printf("var_pile4: %p\n", &var_pile4);
}

void function() {
    int var_pile3;
    printf("var_pile3: %p\n", &var_pile3);
    function2();
}

int main() {
    // int var_pile;
    // int var_pile2;
    function();
    // int *var_tas = malloc(sizeof(int));
    // int *var_tas2 = malloc(sizeof(int));
    // printf("var_data: %p\n", &var_data);
    // printf("var_rodata: %p\n", &var_rodata);
    // printf("var_bss: %p\n", &var_bss);
    // printf("var_pile: %p\n", &var_pile);
    // printf("var_pile2: %p\n", &var_pile2);
    // printf("var_tas: %p\n", &var_tas);
    // printf("var_tas2: %p\n", &var_tas2);
    // printf("main: %p\n", main);

    return 0;
}