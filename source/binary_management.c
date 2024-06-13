#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEEK_END 2

struct wasm_array {
    char* array;
    long* size;
};

struct wasm_array* read_binary_file_struct(const char *path_wasm_file){
    FILE *fileptr;
    char *buffer;
    long filelen;
    struct wasm_array w;
    
    fileptr = fopen(path_wasm_file, "rb");  // Open the file in binary mode
    if (fileptr == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1; // Exit with error code
    }

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (char *)malloc(filelen); // Enough memory for the file
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(fileptr);
        return 1; // Exit with error code
    }

    size_t readSize = fread(buffer, 1, filelen, fileptr); // Read in the entire file
    if (readSize != filelen) {
        fprintf(stderr, "Error reading file.\n");
        free(buffer);
        fclose(fileptr);
        return 1; // Exit with error code
    }

    fclose(fileptr); // Close the file

    char bin_array[filelen];
    strcpy(bin_array, buffer);
    free(buffer);

    w.array = bin_array;
    w.size = &filelen;

    return &w;
};

unsigned char* read_binary_file_array(const char *path_wasm_file){
    FILE *fileptr;
    unsigned char *buffer;
    long filelen;
    
    fileptr = fopen(path_wasm_file, "rb");  // Open the file in binary mode
    if (fileptr == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1; // Exit with error code
    }

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);             // Get the current byte offset in the file
    rewind(fileptr);                      // Jump back to the beginning of the file

    buffer = (unsigned char *)malloc(filelen); // Enough memory for the file
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        fclose(fileptr);
        return 1; // Exit with error code
    }

    size_t readSize = fread(buffer, 1, filelen, fileptr); // Read in the entire file
    if (readSize != filelen) {
        fprintf(stderr, "Error reading file.\n");
        free(buffer);
        fclose(fileptr);
        return 1; // Exit with error code
    }

    fclose(fileptr); // Close the file

    return buffer;
};


// Return the size of the file in bytes
long* get_size_of_file(const char *path_wasm_file)
{
    static long filelen;
    FILE *fileptr;

    fileptr = fopen(path_wasm_file, "rb");  // Open the file in binary mode
    if (fileptr == NULL) {
        fprintf(stderr, "Error opening file.\n");
        return 1; // Exit with error code
    }

    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
    filelen = ftell(fileptr);        // Get the current byte offset in the file

    fclose(fileptr);
    return &filelen;
}