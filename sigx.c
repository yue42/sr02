/* fx.c exemple xlib 
 * routines de creation et modif d'un rectangle
 * compil : cc -c fx.c
 * --> fait un fx.o a utiliser par exemple dans :
 * > cc -o f2 f2.c fx.o -lX11 
 */
/* initrec()   : initialise rectangle rouge
 * i = attendreclic(): boucle attente evenements
 * 	sort avec i = 0 si clic dans rect-fin
 * 	sort avec i = 1 si clic dans grand rect. du bas
 * rectvert(n) : met le rectangle en vert pendant n secondes,
 *               puis le remet a sa couleur precedente.
 * detruitrec(): detruit la fenetre rectangle
 * ecritrec (char *buf, int lng) : crire buf dans la fentre
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>

        Display *xdisplay;
        Window xwindow;
        int xscreen;
        unsigned long xforeground, xbackground;
        XEvent xevent;
        GC xgc;
        Colormap cmap;
        XColor color, colorrgb;

	int toggle = 0; /* 0 = red,  1 = yellow */
	char chaine[80]; /* pour rafraichir fenetre */
	int lngch;	 /* idem */

drawstr (int x, int y, char *str, int lng)
{
	/* Draw string at (x,y) in white */
        if (XAllocNamedColor (xdisplay, cmap, "white", &color, &colorrgb))
            XSetForeground (xdisplay, xgc, color.pixel);
	XDrawString (xdisplay, xwindow, xgc, x, y, str, lng);
}

drawrec (int x, int y, int larg, int hter)
{
	/* Draw rect at (x,y) in white */
        if (XAllocNamedColor (xdisplay, cmap, "white", &color, &colorrgb))
            XSetForeground (xdisplay, xgc, color.pixel);
	XDrawRectangle (xdisplay, xwindow, xgc, x, y, larg, hter);
}

initwind(char *col1, char *col2)
{
        /* Draw rectangle at (25,25), width 300, height, 200 */
        if (XAllocNamedColor (xdisplay, cmap, col1, &color, &colorrgb))
           XSetForeground (xdisplay, xgc, color.pixel);
        XFillRectangle (xdisplay, xwindow, xgc, 25, 25, 400, 200);

	drawstr (30, 60, "_fils_", 6);

	drawrec (100, 30, 100, 50);

	drawrec (105, 35, 90, 40);

	drawrec (225, 30, 95, 50);

	drawstr (230, 60, "_fin_", 5);

	drawrec ( 30, 85,  65, 50);
	drawstr ( 35,105, "_0_", 3);
	drawrec (100, 85, 100, 50);
	drawstr (105,105, "_1_", 3);
	drawrec (225, 85,  95, 50);
	drawstr (230,105, "_2_", 3);
	drawrec (325, 85,  95, 50);
	drawstr (330,105, "_3_", 3);

        XFlush (xdisplay);        /* flush X request queue to server */
}


initrec()
{
        xdisplay = XOpenDisplay ("");        /* connect to the X server */
        if (xdisplay == NULL) {
                fprintf (stderr, "cannot connect to server\n");
                exit (EXIT_FAILURE); }

        xscreen = DefaultScreen (xdisplay);        /* get default screen */

        /* get black and white representation on current screen */        
        xbackground = BlackPixel (xdisplay, xscreen);
        xforeground = WhitePixel (xdisplay, xscreen);

        /* Create window at (ix,iy), width 450, height 250, 
           border width 2, in default root  */
        xwindow = XCreateSimpleWindow (xdisplay,
                DefaultRootWindow(xdisplay), 10, 10, 450, 250, 2,
                xforeground, xbackground);

	/* XMoveWindow (xdisplay,xwindow,ix,iy); ! marche pas ?*/

        if ((int *)xwindow == NULL) {
                fprintf (stderr, "cannot open window\n");
                exit (EXIT_FAILURE); }

        /* ask for exposure event */
        XSelectInput(xdisplay, xwindow, ButtonPressMask | ExposureMask);

        /* pop this window up on the screen */
        XMapRaised (xdisplay, xwindow);

        /* wait for the window showing up before continuing */
        XNextEvent (xdisplay, &xevent);

        /* set graphics context of rectangle to red */
        xgc= XCreateGC (xdisplay, xwindow, 0, 0);
        if (DisplayPlanes (xdisplay, xscreen) != 1) {
            cmap = DefaultColormap (xdisplay, xscreen);
        }

	initwind ("red","yellow");
	return 0;
}


setbigrec (char *col)
{
        /* Fill rectangle at (25,90), width 300, height, 110 */
        if (XAllocNamedColor (xdisplay, cmap, col, &color, &colorrgb))
           XSetForeground (xdisplay, xgc, color.pixel);
        XFillRectangle (xdisplay, xwindow, xgc, 25, 140, 400, 90);
}

setlitrec (char *col)
{
	/* Fill rect at (105,35) in col */
        if (XAllocNamedColor (xdisplay, cmap, col, &color, &colorrgb))
            XSetForeground (xdisplay, xgc, color.pixel);
	XFillRectangle (xdisplay, xwindow, xgc, 105, 35, 90, 40);
}

int attendreclic()
{
while (1) {	/* wait for next event */
	XNextEvent (xdisplay, &xevent);
	switch (xevent.type) {
	case Expose:
		initwind ("red","yellow");
		if (toggle==1) {setbigrec ("yellow"); setlitrec ("red");}
        if (XAllocNamedColor (xdisplay, cmap, "white", &color, &colorrgb))
            XSetForeground (xdisplay, xgc, color.pixel);
	XDrawString (xdisplay, xwindow, xgc, 30, 170, chaine, lngch);
		
        	break;
	case ButtonPress:

/* if mouse button is down inside rectangle fin */
if ((xevent.xbutton.x>=230)&&(xevent.xbutton.x<=300)
&&(xevent.xbutton.y>=30)&&(xevent.xbutton.y<=80)) {
	printf("--clic fin--\n");
	return -1;	
}

/* if mouse button is down inside rectangle jaune */
if ((xevent.xbutton.x>=105)&&(xevent.xbutton.x<=195)
&&(xevent.xbutton.y>=35)&&(xevent.xbutton.y<=80)) {
	printf("--clic toggle--\n");
/* change color */
	if (toggle==0) {
		setbigrec ("yellow"); setlitrec ("red"); toggle=1; }
	else  {
		setbigrec ("red"); setlitrec ("yellow"); toggle=0; }

	break;
}
/* if mouse button is down inside rectangle 0 */
if ((xevent.xbutton.x>=30)&&(xevent.xbutton.x<=95)
&&(xevent.xbutton.y>=85)&&(xevent.xbutton.y<=135)) {
	printf("--clic 0--\n");
	return 0;	
}

/* if mouse button is down inside rectangle 1 */
if ((xevent.xbutton.x>=100)&&(xevent.xbutton.x<=230)
&&(xevent.xbutton.y>=85)&&(xevent.xbutton.y<=135)) {
	printf("--clic 1--\n");
	return 1;	
}

/* if mouse button is down inside rectangle 2 */
if ((xevent.xbutton.x>=230)&&(xevent.xbutton.x<=320)
&&(xevent.xbutton.y>=85)&&(xevent.xbutton.y<=135)) {
	printf("--clic 2--\n");
	return 2;	
}

/* if mouse button is down inside rectangle 3 */
if ((xevent.xbutton.x>=320)&&(xevent.xbutton.x<=420)
&&(xevent.xbutton.y>=85)&&(xevent.xbutton.y<=135)) {
	printf("--clic 3--\n");
	return 3;	
}

/* if mouse button is down inside BIG rectangle */
if ((xevent.xbutton.y>=135) ) {
	printf("--clic 4--\n");
	return 4;	
}

/* if mouse button is elsewhere */
/* return 1 : stop ecoute event, retour prog. main */
	printf("--clic ailleurs -- ignore --\n");
	break;
	}
}
}

ecritrec (char *buf,int lng)
{	int i;
	if (toggle==0) setbigrec ("red");
	else  setbigrec ("yellow");

	/* Draw string at (30,150) in white */
        if (XAllocNamedColor (xdisplay, cmap, "white", &color, &colorrgb))
            XSetForeground (xdisplay, xgc, color.pixel);
	XDrawString (xdisplay, xwindow, xgc, 30, 170, buf, lng);

        XFlush (xdisplay);        /* flush X request queue to server */

/* sav buf dans chaine pour rafraichir si expose event */	
	for (i=0;i<lng;i++) chaine[i]=buf[i];
	lngch = lng;

}

rectvert(int n)
{
	setbigrec ("green");

        XFlush (xdisplay);        /* flush X request queue to server */

	sleep(n);

	//if (toggle==0) setbigrec ("red");
	//else  setbigrec ("yellow");

        XFlush (xdisplay);        /* flush X request queue to server */
	return 0;
}

detruitrec()
{
   XDestroyWindow(xdisplay, xwindow);
   XCloseDisplay (xdisplay);
}


