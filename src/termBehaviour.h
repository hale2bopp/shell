#ifndef ESCAPE_CHARS_H
#define ESCAPE_CHARS_H

const string eraseCursorTillEndOfLine = "\33[K";
const string moveCursorOneLeft = "\33[D";
const string moveCursorOneRight = "\33[C";
const string eraseTillStartOfLine = "\33[2K";
const string moveCursorToBeginningOfLine = "\r";
#define DELETE 127
#define ESCAPE_SEQ 27
#define UP_ARROW 65
#define ENTER 10
#define ASCII_BACKSPACE 8
void moveCursorToBackDisplayPrompt(void);
void eraseLastCharacter(string& s);
void putTerminalInPerCharMode(void);
void putTerminalBackInNormalMode(void);
#endif // ESCAPE_CHARS_H
