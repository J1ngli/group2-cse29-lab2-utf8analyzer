#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

uint8_t is_ascii(char string[]) {
        for (int i = 0; i < strlen(string); ++ i) {
                if (string[i] > 127 || string[i] < 0) {
                        return 0;
                }
        }
        return 1;
}


void capitalize_ascii(char str[]) {
        for (int i = 0; i < strlen(str); ++ i) {
                if (str[i] >= 'a' && str[i] <= 'z') {
                        str[i] -=32;
                }
        }
        
        printf("Uppercased ASCII: %s\n", str);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    // implement the UTF-8 analyzer here
    if (is_ascii(argv[1]) == 1) {
                printf("Valid ASCII: true\n");
        } else {
                printf("Valid ASCII: false\n");
        }

         capitalize_ascii(argv[1]);

        printf("Length in bytes: %d\n", strlen(argv[1]));
}
