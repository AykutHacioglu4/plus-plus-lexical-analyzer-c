#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants
#define TOKEN_CAPACITY 256              // Max size of a token
#define VARIABLE_LIMIT 100              // Max number of variables

// Symbol table to track declared variables
char varList[VARIABLE_LIMIT][21];
int declaredCount = 0;

// Checks if a variable has already been declared
int isAlreadyDeclared(const char *varName) {
    for (int i = 0; i < declaredCount; i++) {
        if (strcmp(varList[i], varName) == 0)
            return 1;
    }
    return 0;
}

// Adds a variable to the symbol table if not already declared
void addToSymbolTable(const char *varName) {
    if (!isAlreadyDeclared(varName) && declaredCount < VARIABLE_LIMIT) {
        strcpy(varList[declaredCount++], varName);
    }
}

// File pointers for input and output
FILE *inputFile, *outputFile;

// Check if a string is a keyword
int checkIfKeyword(const char *word) {
    const char *keywords[] = { "number", "write", "repeat", "times", "newline", "and" };
    for (int i = 0; i < 6; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

// Check if a string is a valid operator
int checkIfOperator(const char *op) {
    return (strcmp(op, ":=") == 0 || strcmp(op, "+=") == 0 || strcmp(op, "-=") == 0);
}

// Checks if a character can start an identifier (must be letter)
int isValidStartChar(char c) {
    return isalpha(c);
}

// Checks if a character can be part of an identifier (letter, digit, underscore)
int isValidIdentifierChar(char c) {
    return isalnum(c) || c == '_';
}

// Reports an error message with the current line number
void logError(const char *message, int lineNumber) {
    fprintf(stderr, "Error [line %d]: %s\n", lineNumber, message);
}

// Main lexical analysis function
void runLexer(const char *inputFileName) {
    char outputFileName[256];
    strcpy(outputFileName, inputFileName);
    char *dot = strrchr(outputFileName, '.');
    if (dot) strcpy(dot, ".lx"); // Replace extension
    else strcat(outputFileName, ".lx");

    // Open input and output files
    inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        perror("Failed to open the input file");
        exit(1);
    }

    outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Failed to create the output file");
        exit(1);
    }

    // Initialization of parsing variables
    char ch;
    char tokenBuffer[TOKEN_CAPACITY];
    int tokenIndex = 0, lineNumber = 1;
    int isExpectingBlock = 0;
    int blockNesting = 0;
    int repeatKeywordLine = -1;
    int afterRepeatKeyword = 0;
    int expectingIdentifier = 0;

    // Character-by-character reading
    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch == '\n') {
            lineNumber++;
            if (isExpectingBlock && !afterRepeatKeyword) {
                char err[100];
                sprintf(err, "Expected '{' after 'repeat ... times' at line %d", repeatKeywordLine);
                logError(err, lineNumber);
                isExpectingBlock = 0;
            }
            afterRepeatKeyword = 0;
            continue;
        }

        if (isspace(ch)) continue;

        // If a declaration expects an identifier
        if (expectingIdentifier) {
            if (!isalpha(ch)) {
                if (isdigit(ch)) {
                    logError("Identifier must begin with a letter", lineNumber);
                } else if (ch == '_') {
                    logError("Identifier cannot begin with an underscore", lineNumber);
                } else {
                    char err[100];
                    sprintf(err, "Invalid identifier start character: '%c'", ch);
                    logError(err, lineNumber);
                }
                while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch));
                expectingIdentifier = 0;
                continue;
            }

            tokenIndex = 0;
            tokenBuffer[tokenIndex++] = ch;
            int isValid = 1;

            // Read rest of identifier
            while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch)) {
                if (tokenIndex >= 20) {
                    logError("Identifier length cannot exceed 20 characters", lineNumber);
                    isValid = 0;
                    while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch));
                    break;
                }
                tokenBuffer[tokenIndex++] = ch;
            }

            tokenBuffer[tokenIndex] = '\0';
            ungetc(ch, inputFile);
            expectingIdentifier = 0;

            if (!isValid) continue;

            if (isAlreadyDeclared(tokenBuffer)) {
                char err[100];
                sprintf(err, "Variable '%s' is declared more than once", tokenBuffer);
                logError(err, lineNumber);
                continue;
            }

            fprintf(outputFile, "Identifier(%s)\n", tokenBuffer);
            addToSymbolTable(tokenBuffer);
            continue;
        }

        // Handle comment: * ... *
        if (ch == '*') {
            int commentEnded = 0;
            int commentStartLine = lineNumber;
            while ((ch = fgetc(inputFile)) != EOF && ch != '\n') {
                if (ch == '*') {
                    commentEnded = 1;
                    break;
                }
            }
            if (!commentEnded) {
                char err[100];
                sprintf(err, "Unclosed comment started on line %d", commentStartLine);
                logError(err, lineNumber);
            }
            if (ch == '\n') lineNumber++;
            continue;
        }

        // Handle string constant
        if (ch == '"') {
            tokenIndex = 0;
            tokenBuffer[tokenIndex++] = ch;
            int stringClosed = 0;
            while ((ch = fgetc(inputFile)) != EOF) {
                if (ch == '\n') lineNumber++;
                tokenBuffer[tokenIndex++] = ch;
                if (ch == '"') {
                    stringClosed = 1;
                    break;
                }
            }
            if (!stringClosed) {
                logError("String literal was not closed", lineNumber);
                continue;
            }
            tokenBuffer[tokenIndex] = '\0';
            fprintf(outputFile, "StringConstant(%s)\n", tokenBuffer);
            continue;
        }

        // Handle identifier or keyword
        if (isalpha(ch)) {
            tokenIndex = 0;
            tokenBuffer[tokenIndex++] = ch;
            int isValid = 1;

            while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch)) {
                if (tokenIndex >= 20) {
                    logError("Identifier too long", lineNumber);
                    isValid = 0;
                    while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch));
                    break;
                }
                tokenBuffer[tokenIndex++] = ch;
            }

            tokenBuffer[tokenIndex] = '\0';
            ungetc(ch, inputFile);
            if (!isValid) continue;

            if (checkIfKeyword(tokenBuffer)) {
                fprintf(outputFile, "Keyword(%s)\n", tokenBuffer);

                if (strcmp(tokenBuffer, "repeat") == 0) {
                    isExpectingBlock = 1;
                    repeatKeywordLine = lineNumber;
                    afterRepeatKeyword = 1;
                } else if (strcmp(tokenBuffer, "times") == 0) {
                    afterRepeatKeyword = 1;
                } else {
                    afterRepeatKeyword = 0;
                }

                if (strcmp(tokenBuffer, "number") == 0) {
                    expectingIdentifier = 1;
                }
            } else {
                if (!isAlreadyDeclared(tokenBuffer)) {
                    char err[100];
                    sprintf(err, "Undeclared variable: '%s'", tokenBuffer);
                    logError(err, lineNumber);
                }
                fprintf(outputFile, "Identifier(%s)\n", tokenBuffer);
                afterRepeatKeyword = 0;
            }
            continue;
        }

        // Identifier starting with underscore
        if (ch == '_') {
            logError("Identifiers cannot start with an underscore", lineNumber);
            while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch));
            continue;
        }

        // Integer constant parsing
        if (isdigit(ch) || ch == '-') {
            tokenIndex = 0;
            int isNegative = (ch == '-');
            tokenBuffer[tokenIndex++] = ch;

            if (isNegative) {
                ch = fgetc(inputFile);
                if (isspace(ch)) {
                    logError("Negative number must not contain space", lineNumber);
                    continue;
                }
                if (ch == '-') {
                    logError("Double minus (--number) is invalid", lineNumber);
                    continue;
                }
                if (!isdigit(ch)) {
                    logError("Invalid format after minus sign", lineNumber);
                    continue;
                }
                tokenBuffer[tokenIndex++] = ch;
            }

            while ((ch = fgetc(inputFile)) != EOF && (isdigit(ch) || ch == '.' || ch == '+' || isalpha(ch) || ch == '_')) {
                if (ch == '.') {
                    logError("Real numbers are not supported", lineNumber);
                    while ((ch = fgetc(inputFile)) != EOF && (isdigit(ch) || ch == '.'));
                    break;
                }
                if (ch == '+') {
                    logError("Plus sign in numbers is not valid", lineNumber);
                    while ((ch = fgetc(inputFile)) != EOF && isdigit(ch));
                    break;
                }
                if (isalpha(ch) || ch == '_') {
                    logError("Number cannot contain letters or underscores", lineNumber);
                    while ((ch = fgetc(inputFile)) != EOF && isValidIdentifierChar(ch));
                    break;
                }
                tokenBuffer[tokenIndex++] = ch;
            }

            tokenBuffer[tokenIndex] = '\0';
            ungetc(ch, inputFile);
            fprintf(outputFile, "IntConstant(%s)\n", tokenBuffer);
            continue;
        }

        // Operators
        if (ch == ':' || ch == '+' || ch == '-') {
            char next = fgetc(inputFile);
            tokenBuffer[0] = ch;
            tokenBuffer[1] = next;
            tokenBuffer[2] = '\0';
            if (checkIfOperator(tokenBuffer)) {
                fprintf(outputFile, "Operator(%s)\n", tokenBuffer);
                afterRepeatKeyword = 0;
                continue;
            } else {
                logError("Invalid operator encountered", lineNumber);
                afterRepeatKeyword = 0;
                continue;
            }
        }

        // Open/close block
        if (ch == '{') {
            fprintf(outputFile, "OpenBlock\n");
            if (isExpectingBlock) {
                isExpectingBlock = 0;
                blockNesting++;
            }
            afterRepeatKeyword = 0;
            continue;
        }

        if (ch == '}') {
            if (blockNesting > 0) {
                fprintf(outputFile, "CloseBlock\n");
                blockNesting--;
            } else {
                logError("Extra closing block detected", lineNumber);
            }
            afterRepeatKeyword = 0;
            continue;
        }

        // Statement end
        if (ch == ';') {
            fprintf(outputFile, "EndOfLine\n");
            afterRepeatKeyword = 0;
            continue;
        }

        // Any other unexpected character
        char err[100];
        sprintf(err, "Unexpected character '%c'", ch);
        logError(err, lineNumber);
        afterRepeatKeyword = 0;
    }

    if (blockNesting > 0) {
        logError("One or more blocks are not closed", lineNumber);
    }

    fclose(inputFile);
    fclose(outputFile);
    printf("Lexical analysis complete. Output saved to %s\n", outputFileName);
}

// Main function: handles command-line arguments and triggers processing
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <sourcefile.plus>\n", argv[0]);
        return 1;
    }

    char fileName[256];
    snprintf(fileName, sizeof(fileName), "%s.plus", argv[1]);
    runLexer(fileName);

    return 0;
}
