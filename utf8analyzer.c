<<<<<<< HEAD
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// Function to get the length of a UTF-8 character based on its first byte
int utf8_char_length(unsigned char byte) {
    if (byte < 0x80) return 1;      // ASCII (0xxxxxxx)
    if (byte < 0xC0) return 0;      // Invalid continuation byte
    if (byte < 0xE0) return 2;      // 2-byte character (110xxxxx)
    if (byte < 0xF0) return 3;      // 3-byte character (1110xxxx)
    if (byte < 0xF8) return 4;      // 4-byte character (11110xxx)
    return 0;                       // Invalid
}

// Function to decode a UTF-8 character to its Unicode codepoint
unsigned int utf8_to_codepoint(const unsigned char* utf8_char, int char_len) {
    unsigned int codepoint = 0;
    
    switch (char_len) {
        case 1:
            codepoint = utf8_char[0];
            break;
        case 2:
            codepoint = ((utf8_char[0] & 0x1F) << 6) | (utf8_char[1] & 0x3F);
            break;
        case 3:
            codepoint = ((utf8_char[0] & 0x0F) << 12) | 
                       ((utf8_char[1] & 0x3F) << 6) | 
                       (utf8_char[2] & 0x3F);
            break;
        case 4:
            codepoint = ((utf8_char[0] & 0x07) << 18) | 
                       ((utf8_char[1] & 0x3F) << 12) | 
                       ((utf8_char[2] & 0x3F) << 6) | 
                       (utf8_char[3] & 0x3F);
            break;
    }
    
    return codepoint;
}

// Function to encode a Unicode codepoint to UTF-8
int codepoint_to_utf8(unsigned int codepoint, unsigned char* utf8_char) {
    if (codepoint <= 0x7F) {
        // 1-byte character
        utf8_char[0] = codepoint;
        return 1;
    } else if (codepoint <= 0x7FF) {
        // 2-byte character
        utf8_char[0] = 0xC0 | (codepoint >> 6);
        utf8_char[1] = 0x80 | (codepoint & 0x3F);
        return 2;
    } else if (codepoint <= 0xFFFF) {
        // 3-byte character
        utf8_char[0] = 0xE0 | (codepoint >> 12);
        utf8_char[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_char[2] = 0x80 | (codepoint & 0x3F);
        return 3;
    } else if (codepoint <= 0x10FFFF) {
        // 4-byte character
        utf8_char[0] = 0xF0 | (codepoint >> 18);
        utf8_char[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        utf8_char[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        utf8_char[3] = 0x80 | (codepoint & 0x3F);
        return 4;
    }
    
    return 0; // Invalid codepoint
}

// Function to check if a codepoint is an animal emoji
bool is_animal_emoji(unsigned int codepoint) {
    // Animal emoji ranges (common ones)
    return (codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||  // Animals & Nature
           (codepoint >= 0x1F980 && codepoint <= 0x1F9AE) ||  // Additional animals
           (codepoint >= 0x1F54A && codepoint <= 0x1F54F) ||  // Some animal-related
           (codepoint == 0x1F40D) ||  // Snake
           (codepoint == 0x1F577) ||  // Spider
           (codepoint == 0x1F578) ||  // Spider web
           (codepoint == 0x1F982) ||  // Scorpion
           (codepoint == 0x1F99F) ||  // Mosquito
           (codepoint == 0x1F9A0) ||  // Microbe
           (codepoint == 0x1F9A2);    // Swan
}

// Function to extract substring containing only the first 6 codepoints
char* get_first_6_codepoints(const char* input) {
    if (!input) return NULL;
    
    int input_len = strlen(input);
    int codepoint_count = 0;
    int byte_index = 0;
    
    // Count bytes needed for first 6 codepoints
    while (byte_index < input_len && codepoint_count < 6) {
        int char_len = utf8_char_length((unsigned char)input[byte_index]);
        
        if (char_len == 0) {
            byte_index++;
            continue;
        }
        
        if (byte_index + char_len > input_len) {
            break;
        }
        
        byte_index += char_len;
        codepoint_count++;
    }
    
    // Allocate memory for the substring
    char* result = malloc(byte_index + 1);
    if (!result) return NULL;
    
    memcpy(result, input, byte_index);
    result[byte_index] = '\0';
    
    return result;
}

// Function to find all animal emojis in the input string
char* get_animal_emojis(const char* input) {
    if (!input) return NULL;
    
    int input_len = strlen(input);
    int byte_index = 0;
    
    // First pass: count total bytes needed for animal emojis
    int total_animal_bytes = 0;
    int temp_byte_index = 0;
    
    while (temp_byte_index < input_len) {
        int char_len = utf8_char_length((unsigned char)input[temp_byte_index]);
        
        if (char_len == 0) {
            temp_byte_index++;
            continue;
        }
        
        if (temp_byte_index + char_len > input_len) {
            break;
        }
        
        unsigned int codepoint = utf8_to_codepoint(
            (const unsigned char*)&input[temp_byte_index], char_len);
        
        if (is_animal_emoji(codepoint)) {
            total_animal_bytes += char_len;
        }
        
        temp_byte_index += char_len;
    }
    
    if (total_animal_bytes == 0) {
        char* empty_result = malloc(1);
        if (empty_result) empty_result[0] = '\0';
        return empty_result;
    }
    
    // Allocate memory for result
    char* result = malloc(total_animal_bytes + 1);
    if (!result) return NULL;
    
    // Second pass: copy animal emojis
    int result_index = 0;
    byte_index = 0;
    
    while (byte_index < input_len) {
        int char_len = utf8_char_length((unsigned char)input[byte_index]);
        
        if (char_len == 0) {
            byte_index++;
            continue;
        }
        
        if (byte_index + char_len > input_len) {
            break;
        }
        
        unsigned int codepoint = utf8_to_codepoint(
            (const unsigned char*)&input[byte_index], char_len);
        
        if (is_animal_emoji(codepoint)) {
            memcpy(&result[result_index], &input[byte_index], char_len);
            result_index += char_len;
        }
        
        byte_index += char_len;
    }
    
    result[result_index] = '\0';
    return result;
}

// Function to get character at index 3 and increment its codepoint by 1
char* get_incremented_char_at_index3(const char* input) {
    if (!input) return NULL;
    
    int input_len = strlen(input);
    int codepoint_index = 0;
    int byte_index = 0;
    
    // Find the character at index 3
    while (byte_index < input_len && codepoint_index < 3) {
        int char_len = utf8_char_length((unsigned char)input[byte_index]);
        
        if (char_len == 0) {
            byte_index++;
            continue;
        }
        
        if (byte_index + char_len > input_len) {
            break;
        }
        
        byte_index += char_len;
        codepoint_index++;
    }
    
    // Check if we found index 3
    if (codepoint_index != 3 || byte_index >= input_len) {
        return NULL;
    }
    
    // Get the character at index 3
    int char_len = utf8_char_length((unsigned char)input[byte_index]);
    if (char_len == 0 || byte_index + char_len > input_len) {
        return NULL;
    }
    
    // Decode the character to its codepoint
    unsigned int original_codepoint = utf8_to_codepoint(
        (const unsigned char*)&input[byte_index], char_len);
    
    // Increment the codepoint by 1
    unsigned int incremented_codepoint = original_codepoint + 1;
    
    // Check for overflow
    if (incremented_codepoint > 0x10FFFF) {
        return NULL;
    }
    
    // Encode the incremented codepoint back to UTF-8
    unsigned char utf8_buffer[5] = {0};
    int new_char_len = codepoint_to_utf8(incremented_codepoint, utf8_buffer);
    
    if (new_char_len == 0) {
        return NULL;
    }
    
    // Allocate memory for the result
    char* result = malloc(new_char_len + 1);
    if (!result) return NULL;
    
    memcpy(result, utf8_buffer, new_char_len);
    result[new_char_len] = '\0';
    
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    // implement the UTF-8 analyzer here
    const char* input = argv[1];
    
    printf("Input string: \"%s\"\n", input);
    printf("Input length (bytes): %zu\n\n", strlen(input));
    
    // 1. Substring of the input string containing only the first 6 codepoints
    char* first_6_codepoints = get_first_6_codepoints(input);
    if (first_6_codepoints) {
        printf("First 6 codepoints: \"%s\"\n", first_6_codepoints);
        printf("Length (bytes): %zu\n\n", strlen(first_6_codepoints));
        free(first_6_codepoints);
    } else {
        printf("First 6 codepoints: (error)\n\n");
    }
    
    // 2. All animal emojis in the input string
    char* animal_emojis = get_animal_emojis(input);
    if (animal_emojis) {
        if (strlen(animal_emojis) > 0) {
            printf("Animal emojis: \"%s\"\n", animal_emojis);
            printf("Length (bytes): %zu\n\n", strlen(animal_emojis));
        } else {
            printf("Animal emojis: (none found)\n\n");
        }
        free(animal_emojis);
    } else {
        printf("Animal emojis: (error)\n\n");
    }
    
    // 3. The code point with value one higher at index 3
    char* incremented_char = get_incremented_char_at_index3(input);
    if (incremented_char) {
        printf("Character at index 3 incremented: \"%s\"\n", incremented_char);
        printf("Length (bytes): %zu\n", strlen(incremented_char));
        free(incremented_char);
    } else {
        printf("Character at index 3 incremented: (not found or error)\n");
    }
    
    return 0;
}
=======
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
>>>>>>> 97474474b07cad19ce3adac367262c5f449fd4cc
