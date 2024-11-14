#ifndef EDIOT_H
#define EDIOT_H

//Includes
//
#define	_DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include<ctype.h>
#include<errno.h>
#include<fcntl.h>
#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/ioctl.h>
#include<sys/types.h>
#include<termios.h> 	//Terminal I/O Interface Header file, give the ability to modify terminal interface settings/specificaiton
#include<time.h>
#include<unistd.h> 	//POSIX Header file, allows us to access user input


//Defines
//
#define EDIOT_VERSION "0.0.1"

#define EDIOT_TAB_STOP 8

#define EDIOT_QUIT_TIMES 3

#define CTRL_KEY(k) ((k) & 0x1f)

enum 	editorKey{
		BACKSPACE = 127,
		ARROW_LEFT = 1000,
		ARROW_RIGHT,
		ARROW_UP,
		ARROW_DOWN,
		DEL_KEY,
		HOME_KEY,
		END_KEY,
		PAGE_UP,
		PAGE_DOWN
};

enum 	editorHighlight{
		HL_NORMAL = 0,
		HL_NUMBER,
		HL_MATCH	
};


//Data
//
typedef struct 	editorRow{
			
			int size;
			int rsize;
			char *chars;
			char *render;
			unsigned char *hl;
} editorRow;

struct 	editorConfiguration{
		int 		cx, cy;
		int 		rx;
		int 		rowoff;
		int 		coloff;
		int 		screenrows;
		int 		screencols;
		int 		numrows;
		editorRow 	*row;
		int		dirty;
		char 		*filename;
		char		statusmsg[80];
		time_t		statusmsg_time;
		struct termios 	termios_orig;
};

struct 	editorConfiguration E;


//Prototypes
//
void 	editorSetStatusMessage(const char *fmt, ...);

void 	editorRefreshScren();

char 	*editorPrompt(char *prompt, void (*callback)(char *, int));


//Terminal
//
void 	die(const char *s);

void 	disableRawMode();

void 	enableRawMode();

int 	editorReadKey();

int 	getCursorPositions(int *rows, int *cols);

int 	getWindowSize(int *rows, int *cols);


//Syntax Highlighting
//
int 	is_seperator(int c);

void 	editorUpdateSyntax(editorRow *row);

int 	editorSyntaxToColor(int hl);


//Row Operations
//
int 	editorRowCxToRx(editorRow *row, int cx);

int 	editorRowRxToCx(editorRow *row, int rx);

void 	editorUpdateRow(editorRow *row);

void 	editorAppendRow(char *s, size_t len);

void 	editorFreeRow(editorRow *row);

void 	editorDelRow(int at);

void 	editorRowInsertChar(editorRow *row, int at, int c);

void 	editorRowAppendString(editorRow *row, char *s, size_t len);

void 	editorRowDelChar(editorRow *row, int at);


//Editor Operations
//
void 	editorInsertChar(int c);

void 	editorInsertNewline(); 

void 	editorDelChar();


//File I/O
//
char 	*editorRowsToString(int *buflen);

void 	editorOpen(char *filename);

void 	editorSave();


//Find
//
void 	editorFindCallback(char *query, int key);

void	editorFind();


//Append Buffer
//
struct 	abuf{
		char *b;
		int len;
};

#define ABUF_INIT {NULL, 0} 	//Append buffer initialization

void 	abAppend(struct abuf *ab, const char *s, int len);

void 	abFree(struct abuf *ab);


//Output
//
void 	editorScroll();

void 	editorDrawRows(struct abuf *ab);

void	editorDrawStatusBar(struct abuf *ab);

void 	editorDrawMessageBar(struct abuf *ab);

void 	editorRefreshScreen();

void 	editorSetStatusMessage(const char *fmt, ...);


//Input
//
char 	*editorPrompt(char *prompt, void (*callback)(char *, int));

void 	editorMoveCursor(int key);

void 	editorProcessKeypress();


//Initialization
//
void 	initializationEditor();

int 	main(int argc, char *argv[]);

#endif
