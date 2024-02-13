# Exercices du cours de chaîne de compilation

## Exercise 1 - pp. 49-51

**Traduire en assembleur ARM le code suivant** 

```
uint32_t a; // global variable
...
for (uint8_t i=0; i<=a; i++)
    g();
```

Solution:
    
```
.text
_start:
    ldr r0, =a
    ldr r1, [r0]    @ charge le contenu de la variable a
    mov r2, #0      @ variable i
loop:
    cmp r2, r1
    bgt _end
    bl  g
    add r2, r2, #1
    b loop
_end:
    b _end
g:
    bx lr

.data
a:
    .word 0x00000003
```

**Même question si i est un unsigned int. Conclusion ?**

La seule différence est que la comparaison se fait sur 32 bits au lieu de 8 bits. Pour **i** comme **uint8_t**, il ne va qu'occuper 1 octect dans la memoire, donc si **a** est plus grand que 255, le code ne fonctionnera pas correctement. Par contre, pour **i** comme **unsigned int**, le code fonctionnera correctement parce que **i** peut avoir le même valeur maximale que la variable **a**.

**Indice : pour voir ce que produit GCC pour ARM :**

```
uint32_t a; // global variable
__attribute__((naked)) void f() {
    for (uint8_t i=0; i<=a; i++)
    g();
}

Puis : arm-none-eabi-gcc -Os -S t.c
```

Le code produit par GCC a une instruction **AND r3, r3, 255** qui fait un masque pour que **i** ne puisse pas dépasser 255, vu qu'il est déclaré comme **uint8_t**.

## Exercise 2 - pp. 49-51

**Traduire en assembleur ARM le code suivant** 

```
// Global variables
uint32_t *a;
uint32_t *b;
uint32_t *c;
…
*a += *c;
*b += *c;
```

Solution:

```
.text
_start:
    ldr r0, =ap      @ charge l'addresse de a
    ldr r1, [r0]    @ charge l'addresse pointé par a
    ldr r2, [r1]    @ charge la valeur pointé par a
    ldr r3, =cp      @ charge l'addresse de c
    ldr r4, [r3]    @ charge la valeur pointé par c
    ldr r5, [r4]    @ charge la valeur pointé par c
    add r2, r2, r5  @ additionne les valeurs
    str r2, [r1]    @ stocke la valeur dans l'adresse pointé par a
    ldr r5, [r4]    @ charge la valeur pointé par c
    ldr r0, =bp      @ charge l'addresse de b
    ldr r1, [r0]    @ charge l'addresse pointé par b
    ldr r2, [r1]    @ charge la valeur pointé par b
    add r2, r2, r5  @ additionne les valeurs
    str r2, [r1]    @ stocke la valeur dans l'adresse pointé par b
_end:
    b _end

.data
ap:
    .global ap
    .word 0x00000000
bp:
    .global bp
    .word 0x0000000A
cp:
    .global cp
    .word 0x0000000B
```

## Exercise 2 (suite) - pp. 49-51

**Comparez avec ce que produit GCC, ainsi :**

```
// Global variables
uint32_t *a;
uint32_t *b;
uint32_t *c;
__attribute__((naked)) void f() {
 *a += *c;
 *b += *c;
}
Puis : arm-none-eabi-gcc -O2 -S t.c
```

Le code généré par GCC est similaire.

**Pourquoi GCC charge-t-il deux fois le contenu de *c au lieu d'une seule ?**

Parce que le compilateur ne sait pas si la valeur de **c** va changer entre les deux instructions, parce que la variable **a** peut pointer pour le même addresse que **c**, donc il charge la valeur deux fois pour être sûr d'avoir la bonne valeur.

## Exercise 1 - pp. 119-122

**Sur un PC Linux x86_64 actuel et par adresses croissantes, dans quel ordre sont stockées les sections suivantes : ```text```, ```data*```, ```rodata*```, ```bss```, ```pile``` et ```tas``` au moment de l’exécution ?**

Solution:

```
#include <stdio.h>
#include <stdlib.h>

int var_data = 5;
const int var_rodata = 10;
int var_bss;

int main() {
    int var_pile = 20;
    int var_pile2 = 30;
    int *var_tas = malloc(sizeof(int));

    printf("var_data: %p\n", &var_data);
    printf("var_rodata: %p\n", &var_rodata);
    printf("var_bss: %p\n", &var_bss);
    printf("var_pile: %p\n", &var_pile);
    printf("var_pile2: %p\n", &var_pile2);
    printf("var_tas: %p\n", &var_tas);
    printf("main: %p\n", main);

    return 0;
}
```

Résultat:

```
var_data: 0x5580e6361010
var_rodata: 0x5580e635f004
var_bss: 0x5580e6361018
var_pile: 0x7fff79510978
var_pile2: 0x7fff7951097c
var_tas: 0x7fff79510980
main: 0x5580e635e189
```

L'ordre de stockage est: **text, rodata, data, bss, pile, tas**.

**Dans quel sens croît la pile ?**

Solution:

```
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
    function();
    return 0;
}
```

Résultat:

```
var_pile3: 0x7ffdcbdd4bd4
var_pile4: 0x7ffdcbdd4bb4
```

La pile croît vers les adresses plus petites. Après chaque appel de fonction, la valeur du pointeur de la pile est décrémentée.

## Exercise 2 - pp. 119-122

**Compilez sans édition de lien ce code-ci (http://bit.ly/2ApXoDl) pour ARM avec une chaîne récente, et avec les optimisations suivantes : Os, O0, O1 et O2. Pour chaque niveau d'optimisation, justifiez la taille des sections de données que vous obtenez.**

Résultat:

**Optimisation Os:**
text    data     bss     dec     hex
163       4       5     172      ac

**Optimisation O0:**
text    data     bss     dec     hex
248       4       5     257     101

**Optimisation O1:**
text    data     bss     dec     hex
170       4       5     179      b3

**Optimisation O2:**
text    data     bss     dec     hex
170       4       5     179      b3

**Justification:**

La colonne "text" indique la taille de la section text, qui contient le code exécutable. La colonne "data" indique la taille de la section data, qui contient les variables globales initialisées. La colonne "bss" indique la taille de la section bss, qui contient les variables globales non initialisées. La colonne "dec" indique la taille totale de la section. La colonne "hex" indique la taille totale de la section en hexadécimal.

Il est possible de voir que la taille de la section text change avec les optimisations, parce que le compilateur va optimiser les instructions du code. Comme l'optimisation -0s est la plus agressif, la taille du code est la moins grande. Les tailles des section data et bss sont les mêmes pour tous les niveaux d'optimisation, parce que les variables globales initialisées et non initialisées sont les mêmes.

**Remplacez ```const char mesg[]``` par ```static const char mesg[]```. Expliquez les différences dans les sections de données par rapport à la question précédente (elles dépendent ici aussi des optimisations).**

**Optimisation Os:**
text    data     bss     dec     hex
149       4       5     158      9e

**Optimisation O0:**
text    data     bss     dec     hex
248       4       5     257     101

**Optimisation O1:**
text    data     bss     dec     hex
156       4       5     165      a5

**Optimisation O2:**
text    data     bss     dec     hex
156       4       5     165      a5

La variable globale qui est declarée comme const peut être sauvegardée dans la section .text ou .rodata. Quand elle est déclarée comme static signifie que la variable est visible seulement dans le fichier où elle est déclarée. Comme la variable n'est pas visible dans les autres fichiers, le compilateur peut utiliser des optimisations plus agressifs. Cela explique pourquoi la taille de la section text est plus petite avec static.

**Remplacez ```const char mesg[]``` par ```const char *mesg```. puis par ```const char * const mesg```. Expliquez les différences dans le code généré et les sections de données par rapport à la question 2.**

Quand on a fait ces modifications, la section .rodata a eu ta taille augmenté car le pointeur constant y a été sauvegardé.

## Exercise 2: indications - pp. 119-122

**Commencez par -O0, et examinez le contenu de .rodata (objdump -s). Pourquoi contient-elle deux fois la même chaîne ? D'ailleurs est-ce bien la même chaîne ?**

```
Contents of section .rodata:
 2000 01000200 00000000 48656c6c 6f20576f  ........Hello Wo
 2010 726c6421 0a004865 6c6c6f20 576f726c  rld!..Hello Worl
 2020 64210000 00000000 78203d20 25642c20  d!......x = %d, 
 2030 79203d20 25642c20 7a203d20 25642c20  y = %d, z = %d, 
 2040 74203d20 25640a00                    t = %d.
 ```

Parce que le compilateur a crée une string avec le caractère de prochaine ligne à la fin de la string et une autre string sans l'en caractère pour utiliser la fonction puts().

**Regardez le code généré (objdump toujours, à vous de trouver la bonne option) : quelle chaîne est utilisée ? À quelle fonction est-elle passée ? Pourquoi n'est-ce pas la même fonction que celle spécifiée dans le code C ? Quel est l'intérêt ? (le man de cette fonction pourra vous aider). La suite va permettre de comprendre à quoi sert l'autre chaîne.**

La chaîne sans le caractère de prochaine ligne a été utilisée avec la fonction puts(), parce que il faut utiliser le caractère de fin de ligne pour que la fonction puts() puisse afficher la chaîne et faire un saut de ligne. De plus, la fonction puts() est plus efficace que la fonction printf().

**Compilez en -O1. Regardez les sections de données et leur contenu. Que remarquez-vous ? Une recherche sur google du nom de la section de données nouvellement apparue vous indiquera son utilité. Avec une édition de lien, vous trouverez quelle chaîne est véritablement utilisée. À vous de faire la suite.**

Il y a une nouvelle section qui s'appelle  ```.rodata.str1 ```. Cette section contient la chaîne de caractères et indique au linker si il y a plus de chaînes identiques, il peut fusioner toutes les chaînes en une seule.