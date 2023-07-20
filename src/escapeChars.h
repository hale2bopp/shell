#ifndef ESCAPE_CHARS_H
#define ESCAPE_CHARS_H

const string eraseTillStartOfLine = "\33[2K";
const string moveCursorToBeginningOfLine = "\r";
#define DELETE 127
#define ESCAPE_SEQ 27
#define UP_ARROW 65
#define ENTER 10
#define ASCII_BACKSPACE 8
static void moveCursorToBackDisplayPrompt(void);

#endif // ESCAPE_CHARS_H