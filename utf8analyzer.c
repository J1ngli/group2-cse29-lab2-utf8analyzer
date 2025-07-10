#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_DEFINE 2048

int utf8_char_length(unsigned char byte) {
    if (byte < 0x80) return 1;
    if (byte < 0xC0) return 0;
    if (byte < 0xE0) return 2;
    if (byte < 0xF0) return 3;
    if (byte < 0xF8) return 4;
    return 0;
}

unsigned int utf8_to_codepoint(const unsigned char* utf8_char, int char_len) {
    unsigned int codepoint = 0;
    switch (char_len) {
        case 1: codepoint = utf8_char[0]; break;
        case 2: codepoint = ((utf8_char[0] & 0x1F) << 6) | (utf8_char[1] & 0x3F); break;
        case 3: codepoint = ((utf8_char[0] & 0x0F) << 12) | ((utf8_char[1] & 0x3F) << 6) | (utf8_char[2] & 0x3F); break;
        case 4: codepoint = ((utf8_char[0] & 0x07) << 18) | ((utf8_char[1] & 0x3F) << 12) | ((utf8_char[2] & 0x3F) << 6) | (utf8_char[3] & 0x3F); break;
    }
    return codepoint;
}

int codepoint_to_utf8(unsigned int codepoint, unsigned char* utf8_char) {
    if (codepoint <= 0x7F) {
        utf8_char[0] = codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        utf8_char[0] = 0xC0 | (codepoint >> 6);
        utf8_char[1] = 0x80 | (codepoint & 0x3F);
        return 2;
    } else if (codepoint <= 0xFFFF) {
        utf8_char[0] = 0xE0 | (codepoint >> 12);
        utf8_char[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_char[2] = 0x80 | (codepoint & 0x3F);
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        utf8_char[0] = 0xF0 | (codepoint >> 18);
        utf8_char[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        utf8_char[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_char[3] = 0x80 | (codepoint & 0x3F);
        return 4;
    }
    return 0;
}

bool is_animal_emoji(unsigned int codepoint) {
    return (codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||
           (codepoint >= 0x1F980 && codepoint <= 0x1F9AE) ||
           (codepoint >= 0x1F54A && codepoint <= 0x1F54F) ||
           (codepoint == 0x1F40D) || (codepoint == 0x1F577) ||
           (codepoint == 0x1F578) || (codepoint == 0x1F982) ||
           (codepoint == 0x1F99F) || (codepoint == 0x1F9A0) ||
           (codepoint == 0x1F9A2);
}

char* get_first_6_codepoints(const char* input) {
    int input_len = strlen(input), count = 0, index = 0;
    while (index < input_len && count < 6) {
        int len = utf8_char_length((unsigned char)input[index]);
        if (len == 0 || index + len > input_len) break;
        index += len;
        count++;
    }
    char* result = malloc(index + 1);
    strncpy(result, input, index);
    result[index] = '\0';
    return result;
}

char* get_animal_emojis(const char* input) {
    int input_len = strlen(input), index = 0;
    char* result = malloc(input_len + 1); // worst case allocation
    int result_index = 0;

    while (index < input_len) {
        int len = utf8_char_length((unsigned char)input[index]);
        if (len == 0 || index + len > input_len) break;

        unsigned int codepoint = utf8_to_codepoint((const unsigned char*)&input[index], len);
        if (is_animal_emoji(codepoint)) {
            memcpy(&result[result_index], &input[index], len);
            result_index += len;
        }
        index += len;
    }

    result[result_index] = '\0';
    return result;
}

char* get_incremented_char_at_index3(const char* input) {
    int input_len = strlen(input), count = 0, index = 0;
    while (index < input_len && count < 3) {
        int len = utf8_char_length((unsigned char)input[index]);
        if (len == 0 || index + len > input_len) break;
        index += len;
        count++;
    }

    if (count != 3 || index >= input_len) return NULL;

    int len = utf8_char_length((unsigned char)input[index]);
    if (len == 0 || index + len > input_len) return NULL;

    unsigned int cp = utf8_to_codepoint((const unsigned char*)&input[index], len);
    cp += 1;

    unsigned char buffer[5] = {0};
    int new_len = codepoint_to_utf8(cp, buffer);
    if (new_len == 0) return NULL;

    char* result = malloc(new_len + 1);
    memcpy(result, buffer, new_len);
    result[new_len] = '\0';
    return result;
}

uint8_t is_ascii(char string[]) {
    for (int i = 0; i < strlen(string); ++i) {
        if ((unsigned char)string[i] > 127) return 0;
    }
    return 1;
}

void capitalize_ascii(char str[]) {
    for (int i = 0; i < strlen(str); ++i) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] -= 32;
        }
    }
    printf("Uppercased ASCII: %s\n", str);
}

int codepoint_number(char str[])
{
        unsigned int count = 0;
        unsigned int index = 0;

        while(str[index] != '\0')
        {
                if((str[index] & 0b11000000) != 0b10000000)
                {
                        count++;
                }
                index++;
        }

        return count - 1; // account for enter key
}

void to_dec(char str[])
{
        uint32_t dec = 0;
        int index = 0;

        printf("Decimal of each codepoint: ");

        while(str[index] != '\0')
        {
                if((str[index] & 0x80) == 0x0)
                {
                        dec = str[index];
                        index++;
                }
                else if((str[index] & 0xE0) == 0xC0)
                {
                        dec = ((str[index] & 0x1F) << 6) | (str[index + 1]);
                        index += 2;
                }
                else if((str[index] & 0xF0) == 0xE0)
                {
                        dec = ((str[index] & 0x0F) << 12) | ((str[index + 1] & 0x3F) << 6) | str[index + 2];
                        index += 3;
                }
                else if((str[index] & 0xF8) == 0xF0)
                {
                        dec = ((str[index] & 0x7) << 18) | ((str[index + 1] & 0x3F << 12)) | ((str[index + 2] & 0x3F) << 6) | str[index + 3];
                        index += 4;
                }
                printf("%u ", dec);
        }
        printf("\n");
}

int num_bytes(char str[])
{
        int count, index = 0;

        printf("Bytes of each codepoint: ");

        while(str[index] != '\0')
        {
                count = 0;
                if((str[index] & 0x80) == 0x0)
                {
                        count++;
                        index++;
                }
                else if((str[index] & 0xE0) == 0xC0)
                {
                        count += 2;
                        index += 2;
                }
                else if((str[index] & 0xF0) == 0xE0)
                {
                        count += 3;
                        index += 3;
                }
                else if((str[index] & 0xF8) == 0xF0)
                {
                        count += 4;
                        index += 4;
                }
                printf("%u ", count);
        }
}

int main(int argc, char* argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    const char* input = argv[1];
    printf("Input string: \"%s\"\n", input);
    printf("Input length (bytes): %zu\n", strlen(input));

    // ASCII Check
    if (is_ascii(argv[1]))
        printf("Valid ASCII: true\n");
    else
        printf("Valid ASCII: false\n");

    // Capitalize if ASCII
    if (is_ascii(argv[1])) {
        char temp[strlen(argv[1]) + 1];
        strcpy(temp, argv[1]);
        capitalize_ascii(temp);
    }

    // First 6 codepoints
    char* first6 = get_first_6_codepoints(input);
    printf("First 6 codepoints: \"%s\"\n", first6);
    free(first6);

    // Animal emojis
    char* animals = get_animal_emojis(input);
    if (strlen(animals) > 0)
        printf("Animal emojis: \"%s\"\n", animals);
    else
        printf("Animal emojis: (none found)\n");
    free(animals);

    // Incremented codepoint at index 3
    char* inc_char = get_incremented_char_at_index3(input);
    if (inc_char) {
        printf("Codepoint at index 3 incremented: \"%s\"\n", inc_char);
        free(inc_char);
    } else {
        printf("Codepoint at index 3 incremented: (not found or error)\n");
    }

    printf("Number of codepoints: %u\n", codepoint_number(*argv));
    to_dec(*argv);
    num_bytes(*argv);
	

    return 0;
}

