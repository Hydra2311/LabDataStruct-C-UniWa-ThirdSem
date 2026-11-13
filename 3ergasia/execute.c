#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    if (system("gcc -c libStack.c -o libStack.o") == 0)
        fprintf(stdout,"\nΕπιτυχής μεταγλώττιση του πηγαίου αρχείου της βιβλιοθήκης.\n");

    if (system("ar rcs libStack.a libStack.o") == 0)
        fprintf(stdout,"\nΕπιτυχής δημιουργία της βιβλιοθήκης\n");

    if (system("gcc stack.c -o mystack -L. -l:libStack.a") == 0)
        fprintf(stdout,"\nΕπιτυχής μεταγλώττιση και σύνδεση κώδικα επίδειξης με βιβλιοθήκη\n");
    
    if (system("./mystack") == 0)
        fprintf(stdout,"\nΕπιτυχής εκτέλεση προγράμματος της βιβλιοθήκης\n");

    return 0;
}