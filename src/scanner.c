#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
    const char* start;
    const char* current; // current is the next char to consume
    int line; // useful for error reporting
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
    scanner.current = source;
    scanner.start = source;
    scanner.line = 1;
}

static bool isAtEnd() {
    return *scanner.current == '\0';
}

Token makeToken(TokenType type) {
    Token token;
    token.type = type;
    token.line = scanner.line;
    token.start = scanner.start;
    token.length = (int)(scanner.current - scanner.start);
    return token;
}

Token errorToken(const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.line = scanner.line;
    token.start = message;
    token.length = (int)(strlen(message));
    return token;
}

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static bool match(char expected) {
    if (isAtEnd()) return false;
    if (*scanner.current != expected) return false;
    scanner.current++; // Consume the expected char
    return true;
}

static char peek() {
    return *scanner.current;
}

static char peekNext() {
    if (isAtEnd()) return '\0';
    return scanner.current[1];
}

static void skipWhitespace() {
    for(;;) {
        char c = peek();
        switch(c) {
            case ' ':
            case '\r':
            case '\t':
                advance();
                break;
            case '\n':
                scanner.line++;
                advance();
                break;
            case '/':
                if (peekNext() == '/') {
                    while (peek() != '\n' && !isAtEnd()) advance();
                } else {
                    return;
                }
                break;
            default:
                return; 
        }
    }
}

static Token string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == "\n") scanner.line++;
        advance();
    }

    if (isAtEnd()) return errorToken("Unterminated string.");

    // Consume the closing quote
    advance();
    return makeToken(TOKEN_STRING);
}

static bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

static bool isAlpha(char c) {
    return  (c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_';
}

static Token number() {
    while(isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {

        // Consume the .
        advance();

        while(isDigit(peek())) advance();
    }

    return makeToken(TOKEN_NUMBER);
}

static TokenType checkKeyword(int start, const char* rest, TokenType type) {
    size_t lengthOfRest = strlen(rest);
    if (scanner.current - scanner.start == start + lengthOfRest &&
        memcmp(scanner.start + start, rest, lengthOfRest) == 0) {
            return type;
        }
    
    return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
    switch (scanner.start[0]) {
        case 'a': return checkKeyword(1, "nd", TOKEN_AND);
        case 'c': return checkKeyword(1, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(1, "lse", TOKEN_ELSE);
        case 'i': return checkKeyword(1, "f", TOKEN_IF);
        case 'n': return checkKeyword(1, "il", TOKEN_NIL);
        case 'o': return checkKeyword(1, "r", TOKEN_OR);
        case 'p': return checkKeyword(1, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(1, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(1, "uper", TOKEN_SUPER);
        case 'v': return checkKeyword(1, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(1, "hile", TOKEN_WHILE);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier() {
    while(isAlpha(peek()) || isDigit(peek())) advance();
    return makeToken(identifierType());
}

Token scanToken() {
    scanner.start = scanner.current;

    skipWhitespace();

    if (isAtEnd()) return makeToken(TOKEN_EOF);

    char c = advance();

    if (isAlpha(c)) return identifier();
    if (isDigit(c)) return number();

    switch (c) {
        case '(': return makeToken(TOKEN_LEFT_PAREN);
        case ')': return makeToken(TOKEN_RIGHT_PAREN);
        case '{': return makeToken(TOKEN_LEFT_BRACE);
        case '}': return makeToken(TOKEN_RIGHT_BRACE);
        case ';': return makeToken(TOKEN_SEMICOLON);
        case ',': return makeToken(TOKEN_COMMA);
        case '.': return makeToken(TOKEN_DOT);
        case '-': return makeToken(TOKEN_MINUS);
        case '+': return makeToken(TOKEN_PLUS);
        case '/': return makeToken(TOKEN_SLASH);
        case '*': return makeToken(TOKEN_STAR);
        case '!':
            return makeToken(
                match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
        case '=':
            return makeToken(
                match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
        case '<':
            return makeToken(
                match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
        case '>':
            return makeToken(
                match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
        case '"': return string();
        }

    return errorToken("Unexpected character.");
}