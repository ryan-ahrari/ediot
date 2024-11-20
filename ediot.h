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
		HL_COMMENT,
		HL_MLCOMMENT,
		HL_KEYWORD1,
		HL_KEYWORD2,
		HL_STRING,
		HL_NUMBER,
		HL_MATCH	
};

#define HL_HIGHLIGHT_NUMBERS (1<<0)

#define HL_HIGHLIGHT_STRINGS (1<<1)


//Data
//
struct editorSyntax{
	char *filetype;
	char **filematch;
	char **keywords;
	char *singleline_comment_start;
	char *multiline_comment_start;
	char *multiline_comment_end;
	int  flags;
};

typedef struct 	editorRow{
			int 		idx;
			int 		size;
			int 		rsize;
			char 		*chars;
			char 		*render;
			unsigned char 	*hl;
			int		hl_open_comment;
} editorRow;

struct 	editorConfiguration{
		int 			cx, cy;
		int 			rx;
		int 			rowoff;
		int 			coloff;
		int 			screenrows;
		int 			screencols;
		int 			numrows;
		editorRow 		*row;
		int			dirty;
		char 			*filename;
		char			statusmsg[80];
		time_t			statusmsg_time;
		struct editorSyntax 	*syntax;
		struct termios 		termios_orig;
};

struct 	editorConfiguration E;


//Filetypes
//
char *C_HL_extensions[] = {".c", ".h", ".cpp", NULL};

char *C_HL_keywords[] 	= {
	"switch", "if", "while", "for", "break", "continue", "return", "else", "struct", "union", "typedef", "static", "enum", "class", "case",  "int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",  "void|", NULL
};

struct editorSyntax HLDB[] = {
	{
		"c",
		C_HL_extensions,
		C_HL_keywords,
		"//", "/*", "*/",
		HL_HIGHLIGHT_NUMBERS | HL_HIGHLIGHT_STRINGS
	},
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))


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

void 	editorSelectSyntaxHighlight();


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
