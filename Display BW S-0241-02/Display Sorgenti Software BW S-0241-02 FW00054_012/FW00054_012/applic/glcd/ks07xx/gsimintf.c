/****************************** GSIMINTF.C *****************************

   Socket interface to the external LCD simulator program.
   Commands and data to/from the simulator is send via sockets messages.

   If the LCD application is developed as a console application (recommended)
   the following library file must be included in the link

     For MSVC 6.x and Borland C++ 5.0 :  WSOCK32.LIB

   Revision date:     250507
   Revision Purpose:  Touch screen simulation interface added.
   Revision date:     080808
   Revision Purpose:  Fix connection error when MSVC unicode string modes were used

   Version 3
   Copyright RAMTEX International ApS 2001-2008

************************************************************************/
#ifndef GHW_PCSIM
#error Only include this file in Gdisp simulation mode
#endif

#ifndef _WIN32
#error Only include this file in WIN32 projects
#endif

#include <stdio.h>
#include <winsock.h>
#include <time.h>
#include <tchar.h>

#include <gdisphw.h>  /* GHW_MINIMIZE_CONSOLE definition */
#include <gsimintf.h>

/***** Start of private communication structures used by the LCD simulator *****/
/***** These data structures are only used by this module                  *****/

/* The the Simulator use <LOW>..<HIGH> repesentation (= 80x86 endian) */
typedef union    /* 8 / 16 bit IO conversion union */
   {
   unsigned short w;          /* unsigned int */
   short i;                   /* int */
   struct
      {
      unsigned char b0;       /* LSB byte */
      unsigned char b1;       /* MSB byte */
      }b;
   } u16;

#define  MAX_INFO_LENGTH 2000

/* LCD commands */
typedef enum
   {
   LCD_SIZE = 256,  /* LCD (re-size) L_SIZE */
   LCD_READY,       /* READY command (responce) */
   LCD_CLR,         /* LCD clr cmd  (no data) */
   LCD_INFO_CLR,    /* Info Clear (no data) */

   LCD_BUF,         /* LCD graphic data buffer L_BUF */
   LCD_BUF_SYNC,    /* LCD graphic data buffer L_BUF followed by wait for READY responce */
   LCD_INFO,        /* Info C-string */
   LCD_GETKEY,      /* Get key command */
   LCD_KEY,         /* Key (responce) */
   LCD_SIZE_COLOR,  /* (not used by b&w lib) */
   LCD_PALETTE,     /* (not used by b&w lib) */
   LCD_GETTOUCH,    /* Get touch command */
   LCD_TOUCH        /* Touch (responce) */
   } LCD_COMMAND;

typedef struct
   {
   u16 len;                    /* Length of total data package (including the length itself) */
   u16 cmd;                    /* Command LCD_COMMAND */
   } L_HDR;

/* Structure for transmission of size info */
typedef struct
   {
   u16 w;    /* New width  in LCD pixels */
   u16 h;    /* New height in LCD pixels */
   } L_SIZE;

/* Structure for transmission of a coordinate pair */
typedef struct
   {
   u16 x1;
   u16 y1;
   u16 x2;
   u16 y2;
   } L_XY;

/* Socket LCD update structure */
typedef struct
   {
   L_XY pos;              /* Pixel coordinates for upper left and lower right corner */
   unsigned char dat[1];  /* (start of) LCD pixel data (8 bit horizontal, byte aligned) */
   } L_LCD;

/* Socket LCD data union */
typedef union
   {
   L_SIZE size;             /* New LCD size */
   L_LCD  graph;            /* LCD data */
   unsigned char buf[1];    /* (start of) generic buffer */
   char str[1];             /* (start of) C string for info message */
   } L_BUF;

/* Socket LCD tx structure */
typedef struct
   {
   L_HDR hdr;
   L_BUF buf;
   } L_TXBUF;

/***************** end communication structures *******************/

/***************** Private data *******************/
/*
   The default socket address setup parameters for the LCDSERV server are:
      Name_or_IP : localhost
      Port       : 3000

   If another socket address is wanted the two address definitions
   GSimServer and  GSimPort  below should be modified accordingly.

   When the socket address setup is modified from the default then
   LCDSERV.EXE must be started with three command line parameters:
           0  address_name  port
   similar to:
      LCDSERV.EXE 0 localhost 3000
   or
      LCDSERV.EXE 0 127.0.0.1 3000
*/
static char *GSimServer = {"localhost"};
static unsigned short GSimPort   = 3000;

static SOCKET GSimSocket = INVALID_SOCKET;
static L_TXBUF *txbuf = NULL;        /* transmission buffer */
static unsigned char *lcdbuf = NULL; /* LCD screen copy */
static unsigned short lcdw = 0;      /* LCD screen pixel size (initiate defaults) */
static unsigned short lcdh = 0;
static unsigned short lcdbw = 0;     /* LCD screen byte size */
static unsigned short ilty = 0;      /* "Dirty area" pixel coordinates */
static unsigned short iltx = 0;
static unsigned short irbx = 0;
static unsigned short irby = 0;
static unsigned short oldkey = 0;
static unsigned char  GSimErr = 1;

#define BUFSIZE 80                  /* buffer size for console window titles */
static _TCHAR console_title[BUFSIZE];   /* hold console window title (application name) */
static HWND console_hwnd;           /* handle to console window */


/* Helper macro for displaying errors */
#define PRINTERROR(s)   \
   { \
   if (console_hwnd) \
      ShowWindow( console_hwnd, SW_RESTORE ); /* Restore console window */ \
   fprintf(stderr,"\n%s",s); \
   Sleep(2000); /* Show message in at least 2 seconds */ \
   }

/***************** Private functions *******************/

/*
   This module finds the handle to the console window so it can be
   minimized automatically.
   The trick used is to give the console window a unique name temporarily
   so we can use FindWindow() to find it and get its handle for further use.
*/
static void get_console_hwnd(void)
   {
   #ifdef GHW_MINIMIZE_CONSOLE
   _TCHAR NewWindowTitle[BUFSIZE];            /* contains fabricated WindowTitle */
   GetConsoleTitle(console_title, BUFSIZE-1); /* fetch current window title */
   wsprintf(NewWindowTitle,_T("%d/%d"),       /* format a "unique" NewWindowTitle */
      GetTickCount(),GetCurrentProcessId());
   SetConsoleTitle(NewWindowTitle);         /* Temp change current window title */
   Sleep(40);                               /* ensure window title has been updated */
   console_hwnd=FindWindow(NULL, NewWindowTitle); /* look for NewWindowTitle */
   SetConsoleTitle(console_title);          /* restore original window title */

   #ifdef  _UNICODE
      {
      /* MSVC wide-char compilation mode used, compress info string to a char string for simulator */
      _TCHAR *tcp;
      unsigned char *cp;
      for(tcp = &console_title[0], cp = (unsigned char *) (&console_title[0]); (*tcp & 0xff) != 0; cp++, tcp++)
         {
         if ((*tcp) >= ((_TCHAR) 0xfe))
            { /* Extended file names are not supported */
            strcpy((char *)console_title,"LCD application");
            return;
            }
         *cp = (unsigned char) (*tcp);
         }
      *cp = 0;
      }
   #endif
   #else
   strcpy((char *)console_title,"LCD application");
   console_hwnd = 0;
   #endif
   }

/*
   Init connection to LCD simulator server.
   The simulator server is supposed to be up running already.
*/
static unsigned char socket_init(void)
   {
   int nRet;
   WSADATA wsaData;
   WORD wVersionRequested;
   LPHOSTENT lpHostEntry;
   SOCKADDR_IN saServer;
   GSimErr = 1;

   /* Initialize WinSock and check the version */
   wVersionRequested = MAKEWORD(1,1);
   WSAStartup(wVersionRequested, &wsaData);
   if (wsaData.wVersion < wVersionRequested)
      {
      fprintf(stderr,"\nERROR Wrong WinSock version\n");
      return 1;
      }

   /* Find the server */
   if ((lpHostEntry = gethostbyname(GSimServer)) == NULL)
      {
      PRINTERROR("ERROR Could not locate server\n LCDSERV.EXE must be up running on this machine in advance");
      return 1;
      }

   /* Create a TCP/IP stream socket */
   if((GSimSocket = socket(PF_INET,          /* Address family */
                         SOCK_STREAM,        /* Socket type */
                         IPPROTO_TCP))       /* Protocol */
       == INVALID_SOCKET)
      {
      PRINTERROR("ERROR socket creation");
      return 1;
      }

   /* Fill in the address structure */
   saServer.sin_family = PF_INET;
   saServer.sin_addr = *((LPIN_ADDR)*lpHostEntry->h_addr_list); /* Server's address */
   saServer.sin_port = htons(GSimPort);                         /* Port number */

   /* connect to the server */
   nRet = connect(GSimSocket,               /* Socket */
                 (LPSOCKADDR)&saServer,    /* Server address */
                  sizeof(struct sockaddr)); /* Length of server address structure */
   if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("ERROR connecting to LCD simulator failed\n LCDSERV.EXE must be up running on this machine");
      closesocket(GSimSocket);
      GSimSocket = 0;
      return 1;
      }
   get_console_hwnd();            /* Get handle so we can minimize window */
   if (console_hwnd)
      ShowWindow( console_hwnd, SW_MINIMIZE );
   GSimErr = 0;
   return 0;
   }

/*
   Wait for one byte READY responce
   Return 1 if error,
   Return 0 if no-error
*/
static unsigned char wait_ready(void)
   {
   int nRet;
   txbuf->hdr.len.w = 0xFFFF; /* Make some values to be overwritten */
   txbuf->hdr.cmd.w = 0;
   nRet = recv(GSimSocket, (char *) txbuf, 4, 0);
   if (nRet == SOCKET_ERROR )
      {
      closesocket(GSimSocket);
      GSimSocket = 0;
      GSimErr = 1;
      return 1;
      }
   return (unsigned char)(((nRet == 4) && (txbuf->hdr.len.w == 0) && (txbuf->hdr.cmd.w == LCD_READY)) ? 0 : 1);
   }

/*
   Transmit data to socket
   Buffer length len is allowed to be be zero (i.e. command only).
   A buf NULL pointer indicates that txbuf is used.

   The socket is closed if any error is detected.

   Return ==0 if no errors
   Return !=0 if errors
*/
static unsigned char GSimTx(unsigned short cmd, unsigned short len, const unsigned char *buf)
   {
   int nRet, txlen;
   if ((GSimErr == 1) || (GSimSocket == INVALID_SOCKET))
      return 1;

   GSimErr = 1;
   /* Make header */
   txbuf->hdr.len.w = len;
   txbuf->hdr.cmd.w = cmd;

   /* Transmit header only or header + txbuf */
   txlen = buf ? sizeof(L_HDR) : len + sizeof(L_HDR);
   nRet = send(GSimSocket, (const char *) txbuf, txlen, 0);
   if (nRet == SOCKET_ERROR)
      {
      PRINTERROR("ERROR socket send");
      closesocket(GSimSocket);
      return 1;
      }
   if (nRet != txlen )
      {
      PRINTERROR("ERROR socket in number of bytes send");
      closesocket(GSimSocket);
      return 1;
      }

   if (buf)
      {
      /* Transmit foreign buffer */
      nRet = send(GSimSocket, (const char *) buf, len, 0);
      if (nRet == SOCKET_ERROR)
         {
         PRINTERROR("ERROR socket send");
         closesocket(GSimSocket);
         return 1;
         }
      if (nRet != len )
         {
         PRINTERROR("ERROR socket in number of bytes send");
         closesocket(GSimSocket);
         return 1;
         }
      }
   GSimErr = 0;
   return 0;
   }

/***************** Public functions *******************/

/*
  Init connection to simulator for a LCD display of w*h pixels
     Return ==0 if no errors
     Return !=0 if errors
*/
unsigned char GSimInit(unsigned short w, unsigned short h)
   {
   unsigned int size;
   oldkey = 0;
   if (GSimSocket != INVALID_SOCKET)
      {
      /* A LCD simulator connection already exist */
      if ((lcdw == w) && (lcdh == h))
         {
         /* The LCD size is the same so just clear the content */
         if ((GSimGClr() == 0) && (GSimPutsClr() == 0))
            {  /* Communication was ok */
            char str[BUFSIZE+15];
            strcpy(str, (char *) console_title);
            strncat(str, " re-init.", sizeof(str));
            return GSimPuts(str);
            }
         }
      GSimClose(); /* Different size or communication failed, so clear and retry */
      }

   /* Create socket connection */
   if (socket_init())
      return 1;

   /* Allocate lcd buffer */
   if (lcdbuf != NULL)
      free(lcdbuf);
   lcdbw = (unsigned short)((w+7)/8);
   lcdw = w;
   lcdh = h;
   if ((lcdbuf = (unsigned char *) calloc(lcdbw,lcdh)) == NULL)
      {
      PRINTERROR("ERROR during buffer allocation");
      GSimClose();
      return 1;
      }

   /* Make "dirty-area" rectangle empty */
   iltx = 1;
   ilty = 1;
   irbx = 0;
   irby = 0;

   /* Allocate transmission buffer */
   if (txbuf != NULL)
      free(txbuf);

   size = MAX_INFO_LENGTH + sizeof(L_TXBUF);
   if (size < (lcdbw*lcdh + sizeof(L_TXBUF)))
       size = lcdbw*lcdh + sizeof(L_TXBUF);

   if ((txbuf = (L_TXBUF *) malloc(size)) == NULL)
      {
      PRINTERROR("ERROR during buffer allocation");
      GSimClose();
      return 1;
      }

   /* Wait for server responce */
   if (!wait_ready())
      {
      /* Set/update LCD screen size */
      txbuf->buf.size.w.w = w;
      txbuf->buf.size.h.w = h;
      if (!GSimTx(LCD_SIZE, sizeof(L_SIZE), NULL))
         {
         if (!wait_ready())
            {
            char str[BUFSIZE+15];
            strcpy(str,"\r");
            strncat(str, (char *) console_title, sizeof(str));
            strncat(str, " connected.", sizeof(str));
            GSimPuts(str);
            return 0;
            }
         }
      }

   PRINTERROR("ERROR no server responce");
   GSimClose();
   return 1;
   }

/*
   Close connection to LCD display simulator
   If the program is just terminated the connection will be closed autoamtically.
*/
void GSimClose(void)
   {
   /* Release buffers */
   if (lcdbuf)
      free(lcdbuf);
   lcdbuf = NULL;
   if (txbuf)
      free(txbuf);
   txbuf = NULL;
   lcdw = lcdh = 0;
   /* Close connection */
   if (GSimSocket != INVALID_SOCKET)
      {
      closesocket(GSimSocket);
      GSimSocket = INVALID_SOCKET;
      /* Release WinSock */
      WSACleanup();
      }

   console_hwnd = 0;
   oldkey = 0;
   }

/*
   Return the current error state
*/
unsigned char GSimError(void)
   {
   return GSimErr;
   }

/*
   Clear LCD simulator info window and the keyboard queue

   Return ==0 if no errors
   Return !=0 if errors
*/
unsigned char GSimPutsClr(void)
   {
   return GSimTx(LCD_INFO_CLR, 0, NULL);
   }

/*
   Clear LCD simulator Graphic Window

   Return ==0 if no errors
   Return !=0 if errors
*/
unsigned char GSimGClr(void)
   {
   return GSimTx(LCD_CLR, 0, NULL);
   }

/*
   Transmit an information string to the
   LCD simulator info window

   Return ==0 if no errors
   Return !=0 if errors
*/
unsigned char GSimPuts(const char *str)
   {
   if ((str == NULL) || (*str == 0))
      return 0;
   return GSimTx(LCD_INFO, (unsigned short) (strlen(str)+1), (const unsigned char *)str);
   }


/*
   Graphic data is transfered bit wise to the LCD buffer so the simulator
   interface is independent of any particular byte orientation in the LCD
   controller.

   The "dirty" area of the buffer must later be flushed to the LCD simulator

   Returns 1 if parameter errors or simulator is uninitialized
   Returns 0 if no errors.
*/
unsigned char GSimWrBit(unsigned short x, unsigned short y, unsigned char val)
   {
   static const unsigned char mask[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

   if ((lcdbuf == NULL) || (x >= lcdw) || (y >= lcdh))
      return 1;  /* Parameter error or missing initialization */

   if (val != 0)
      lcdbuf[x/8+lcdbw*y] |= mask[x & 0x7];
   else
      lcdbuf[x/8+lcdbw*y] &= ~mask[x & 0x7];

   /* Update "dirty area" markers */
   if(  irbx < iltx )
       iltx = irbx = x;
   else if( x < iltx )
       iltx = x;
   else if( x > irbx )
       irbx = x;

   if( irby < ilty)
       ilty = irby = y;
   else if( y < ilty )
       ilty = y;
   else if( y > irby )
       irby = y;
   return 0;
   }

/*
   Flush "dirty" part of graphic buffer to the LCD simulator

   Return 1 if simulator is uninitialized or communication errors
   Return 0 if no errors.
*/
unsigned char GSimFlush(void)
   {
   unsigned short x,y,i;
   if(lcdbuf == NULL)
      return 1;
   if((irby < ilty ) || ( irbx < iltx ))
      return 0; /* No new data to flush */

   /* Align update retangle to horizontal byte limits */
   iltx &= ~(0x7);
   irbx |= 0x7;
   /* Copy coordinates to buffer header */
   txbuf->buf.graph.pos.x1.w = iltx;
   txbuf->buf.graph.pos.y1.w = ilty;
   txbuf->buf.graph.pos.x2.w = irbx;
   txbuf->buf.graph.pos.y2.w = irby;

   /* Copy dirty area data to transmission buffer */
   iltx /= 8;
   irbx /= 8;
   i=0;
   for (y = ilty; y <= irby; y++)
      for (x = iltx; x <= irbx; x++)
         txbuf->buf.graph.dat[i++] = lcdbuf[x+y*lcdbw];

   /* transmit data and wait for responce */
   GSimTx(LCD_BUF_SYNC, (unsigned short)(sizeof(L_LCD) + i - 1), NULL);
   if (GSimErr == 0)
      wait_ready();

   /* Clear dirty area */
   iltx = 1;
   ilty = 1;
   irbx = 0;
   irby = 0;
   return GSimErr;
   }

/*
   Get a keycode or 0 if no keys are pending in
   the servers keyboard queue
*/
static int get_key(void)
   {
   int nRet;
   GSimTx(LCD_GETKEY, 0, NULL);

   txbuf->hdr.len.w = 0xFFFF; /* Make some values to be overwritten */
   txbuf->hdr.cmd.w = 0;
   nRet = recv(GSimSocket, (char *) txbuf, 6, 0);
   if (nRet == SOCKET_ERROR )
      {
      closesocket(GSimSocket);
      GSimSocket = 0;
      return 1;
      }
   if ((nRet == 6) && (txbuf->hdr.len.w == 2) && (txbuf->hdr.cmd.w == LCD_KEY))
      {
      nRet = txbuf->buf.buf[0] * 256 + txbuf->buf.buf[1];
      return nRet;
      }
   else
      {
      PRINTERROR("ERROR during keyboard poll");
      return -1;
      }
   }

/*
   Get a key from LCD server
*/
unsigned short GSimKbGet(void)
   {
   int key;
   if ((oldkey != 0) || (GSimErr != 0))
      {
      key = oldkey;
      oldkey = 0;
      return (unsigned short) key;
      }
   while ((key = get_key()) >= 0)
      {
      if (key != 0)
         return (unsigned short) key;
      Sleep(20);   /* Give more processing time to other applications */
      }
   return 0;
   }

/*
   Check a key from LCD server
*/
unsigned short GSimKbHit(void)
   {
   int key;
   if (GSimErr != 0)
      return 0;
   if (oldkey != 0)
      return 1;
   if ((key = get_key()) < 0)
      return 0;  /* Some server connection error */
   oldkey = (unsigned short) key;
   return (unsigned short)((oldkey != 0) ? 1 : 0);
   }

/*
   Simulates a 10ms timerstamp counter
*/
unsigned long GSimTimestamp(void)
   {
   return (((unsigned long) clock())*100L)/CLOCKS_PER_SEC;
   }


/*
   Simulates a touch screen input.
   Returns 1 if a touch change (an edge or pressed coordinate change) has been detected
   Returns 0 if the touch level is unchanged (unpressed, or pressed and same position).

   Edge  = 1, Touch screen pressed changed state.
   Edge  = 0, No touch state change.
   Level = 1, Touch screen pressed (x,y values is a (new) valid position)
   Level = 0, No touch (x,y values is the position where last touch stopped)
*/
unsigned char GSimTouchGet(unsigned char *edgep, unsigned char *levelp, unsigned short *xp, unsigned short *yp)
   {
   int nRet;
   static unsigned short oldx = 0;
   static unsigned short oldy = 0;
   unsigned short x;
   unsigned short y;

   GSimTx(LCD_GETTOUCH, 0, NULL);

   txbuf->hdr.len.w = 0xFFFF; /* Make some values to be overwritten */
   txbuf->hdr.cmd.w = 0;
   nRet = recv(GSimSocket, (char *) txbuf, 4+6, 0);
   if (nRet == SOCKET_ERROR )
      {
      closesocket(GSimSocket);
      GSimSocket = 0;
      return 0;
      }
   if ((nRet == 4+6) && (txbuf->hdr.len.w == 6) && (txbuf->hdr.cmd.w == LCD_TOUCH))
      {
      if (edgep != NULL)
         *edgep = txbuf->buf.buf[0];                   /* level */
      if (levelp != NULL)
         *levelp = txbuf->buf.buf[1];                  /* level */
      x = txbuf->buf.buf[2] * 256 + txbuf->buf.buf[3]; /* x pos */
      y = txbuf->buf.buf[4] * 256 + txbuf->buf.buf[5]; /* y pos */
      if (xp != NULL)
         *xp = x;
      if (yp != NULL)
         *yp = y;
      nRet = ((txbuf->buf.buf[0] != 0) || (x != oldx) || (y != oldy)) ? 1 : 0; /* Change detection */
      oldx = x;
      oldy = y;
      return (unsigned char) nRet;
      }
   else
      {
      PRINTERROR("ERROR during touch poll");
      return 0;
      }
   }


