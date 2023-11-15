#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 256

void extractStrings(const char *filename, const char *outputFilename) {

    FILE *file = fopen(filename, "rb");

    if (file == NULL) {

        perror("Error opening file");
        
		exit(EXIT_FAILURE); // EXIT_FAILURE is portable so i better use it yuh

    }

	FILE *outputFile = stdout; // default to stdout if no output file specified
	
    if (outputFilename != NULL) {

        outputFile = fopen(outputFilename, "w");

        if (outputFile == NULL) {

            perror("Error opening output file");

            exit(EXIT_FAILURE);

        }
		
    }

    char buffer[MAX_STRING_LENGTH];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {

		for (size_t i = 0; i < bytesRead; i++) {

			if (outputFilename != NULL) { // check if output file was provided

				if (buffer[i] >= 32 && buffer[i] <= 126) {
					
					fprintf(outputFile, "%c", buffer[i]); // write to output file
				
				} else if (buffer[i] == '\0' && i > 0) {

					fprintf(outputFile, "\n"); // write to output file
											   
				} else if (buffer[i] == '\0' && i == 0) {

					continue; // skip empty ones
				}

			} else {

				// no output file specified, print to console
				if (buffer[i] >= 32 && buffer[i] <= 126) {

					printf("%c", buffer[i]); // printable ascii characters

				} else if (buffer[i] == '\0' && i > 0) {

					printf("\n"); // null-terminated strings
					
				} else if (buffer[i] == '\0' && i == 0) {

					continue; // skip empty ones

				}

			}

		}

	}

    fclose(file);

}

int main(int argc, char *argv[]) {

    if (argc < 2 || argc > 4) {

        fprintf(stderr, "Usage: %s <executable> [-o / --output <filename>] [-h / --help]\n", argv[0]);

        exit(EXIT_FAILURE);

    }

    const char *filename = NULL;
    const char *outputFilename = NULL;

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {

            printf("ASE (ASCII string extractor)\n\nUsage:\n    %s <executable>\n\nOptional arguments:\n    [-o, --output] <filename>: Write extracted strings into a file.\n    [-h, --help]: Write this help message and exit.\n\nHomepage: https://github.com/morphineaddicted/ase\nSubmit bugs here: https://github.com/morphineaddicted/ase/issues\n", argv[0]);

            exit(EXIT_SUCCESS);

        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {

            if (i + 1 < argc) {

                outputFilename = argv[i + 1];

                i++; // skip the next argument (output filename)
				
            } else {

                fprintf(stderr, "Error: Output filename not provided after %s\n", argv[i]);

                exit(EXIT_FAILURE);

            }
        } else {

            // assume that any other argument is the filename
            filename = argv[i];

        }

    }

    if (filename == NULL) {

        // display an error if no filename is provided
        fprintf(stderr, "Error: Input filename not provided.\n");

        exit(EXIT_FAILURE);

    }

    // skip the file-related operations if in "help" mode
    if (outputFilename == NULL) {

        extractStrings(filename, NULL);

    } else {

        extractStrings(filename, outputFilename);

    }

    return 0;
}

