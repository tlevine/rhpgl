typedef struct {
	FILE *texfp;
	char filename[1024];
	int pageno;
	int landscape;
	double width;
	double height;
	double pagewidth;
	double pageheight;
	double xlast;
	double ylast;
	double clipleft, clipright, cliptop, clipbottom;
	double clippedx0, clippedy0, clippedx1, clippedy1;

	double cex;
	double srt;
	int lty;
	int lwd;
	int col;
	int fg;
	int bg;
	int fontsize;
	int fontface;
	Rboolean debug;
	Rboolean xmlHeader;
	Rboolean onefile; /* drop headers etc*/

} HPGLDesc;

static void HPGL_Activate( pDevDesc);
static void HPGL_Circle(double x, double y, double r, const pGEcontext gc,
		pDevDesc dd);
static void HPGL_Clip(double, double, double, double, pDevDesc);
static void HPGL_Close( pDevDesc);
static void HPGL_Deactivate( pDevDesc);
static void HPGL_Line(double x1, double y1, double x2, double y2,
		const pGEcontext gc, pDevDesc dd);
static Rboolean HPGL_Locator(double*, double*, pDevDesc);
static void HPGL_Mode(int, pDevDesc);
static void HPGL_NewPage(const pGEcontext gc, pDevDesc dd);
static Rboolean HPGL_Open( pDevDesc, HPGLDesc*);
static void HPGL_Polygon(int n, double *x, double *y, const pGEcontext gc,
		pDevDesc dd);
static void HPGL_Polyline(int n, double *x, double *y, const pGEcontext gc,
		pDevDesc dd);
static void HPGL_Rect(double x0, double y0, double x1, double y1,
		const pGEcontext gc, pDevDesc dd);
static void HPGL_Size(double *left, double *right, double *bottom, double *top,
		pDevDesc dd);

static double HPGL_StrWidth(const char *str, const pGEcontext gc, pDevDesc dd);
static void HPGL_Text(double x, double y, const char *str, double rot,
		double hadj, const pGEcontext gc, pDevDesc dd);
static void HPGL_MetricInfo(int c, const pGEcontext gc, double* ascent,
		double* descent, double* width, pDevDesc dd);


static void HPGL_Activate(pDevDesc dd) {
}

static void HPGL_Deactivate(pDevDesc dd) {
}

static void HPGL_MetricInfo(int c, const pGEcontext gc, double* ascent,
		double* descent, double* width, pDevDesc dd) {
		
	//	Rboolean Unicode = mbcslocale && (gc->fontface != 5);
    //     if (c < 0) { Unicode = TRUE; c = -c; }
    //     if(Unicode) UniCharMetric(c, ...); else CharMetric(c, ...);
	/* metric information not available => return 0,0,0 */
	*ascent = 0.0;
	*descent = 0.0;
	*width = 0.0;
}

static Rboolean HPGL_Open(pDevDesc dd, HPGLDesc *ptd) {
	ptd->debug = FALSE;
	if (!(ptd->texfp = (FILE *) fopen(R_ExpandFileName(ptd->filename), "w")))
		return FALSE;

	fprintf(ptd->texfp, "IN;IP;");
	fprintf(ptd->textfp, "SP%d;", dd->startcol);
	// dd->startfill;
	return TRUE;
}

static void HPGL_NewPage(const pGEcontext gc, pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;
	fprintf(ptd->texfp, "PG;");
}

static void HPGL_Line(double x1, double y1, double x2, double y2,
		const pGEcontext gc, pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;
	fprintf(ptd->texfp, "PU%d,%d;PD%d,%d;", x1, y1, x2, y2);
}

static void HPGL_Polyline(int n, double *x, double *y, const pGEcontext gc,
		pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;
    fprintf(ptd->texfp, "PU;PD");
	for (i = 0; i < n; i++) {
        if (i==0) {
            fprintf(ptd->texfp, "%d,%d", x[i], y[i]);
        } else {
            fprintf(ptd->texfp, ",%d,%d", x[i], y[i]);
        }
	}
    fprintf(ptd->texfp, ";");
}

static void HPGL_Rect(double x0, double y0, double x1, double y1,
		const pGEcontext gc, pDevDesc dd) {
	double tmp;
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;

	fprintf(ptd->texfp, "PA%d,%d;", x0, y0);
    // gc->density is lines per inch
    // Second argument of FT is distance between lines in plotter units
    fprintf(pty->texfp, "FT3,%d,%d;", (40 * 250)/(gc->density), gc->angle);
    // gc->fill,
    fprintf(pty->texfp, "SP%d;", gc->col);
    fprintf(pty->texfp, "PW%d;", gc->lwd);
	fprintf(ptd->texfp, "RA%d,%d;", x0, y0);
}

static void HPGL_Circle(double x, double y, double r, const pGEcontext gc,
		pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;

	fprintf(ptd->texfp, "PA%d,%d;", x, y);
    fprintf(pty->texfp, "FT%d;", gc->fill, 1 - (gc->lty));
    fprintf(pty->texfp, "SP%d;", gc->col);
    fprintf(pty->texfp, "PW%d;", gc->lwd);
	fprintf(ptd->texfp, "CI%d;", r);
}

static void HPGL_Polygon(int n, double *x, double *y, const pGEcontext gc,
		pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;

    fprintf(ptd->texfp, "PA;PM0;PD");
	for (i = 0; i < n; i++) {
        if (i==0) {
            fprintf(ptd->texfp, "%d,%d", x[i], y[i]);
        } else {
            fprintf(ptd->texfp, ",%d,%d", x[i], y[i]);
        }
	}
    fprintf(ptd->texfp, ";PM2;EP;");
}

static void HPGL_Text(double x, double y, const char *str, double rot,
		double hadj, const pGEcontext gc, pDevDesc dd) {
	HPGLDesc *ptd = (HPGLDesc *) dd->deviceSpecific;

	fprintf(ptd->texfp, "DT%c;", 3);
	fprintf(ptd->texfp, "LB");
	fprintf(ptd->texfp, str);
	fprintf(ptd->texfp, ";");
}

static Rboolean SVG_Locator(double *x, double *y, pDevDesc dd) {
	fprintf(ptd->texfp, "OD;");
	return FALSE;
}

Rboolean HPGLDeviceDriver(pDevDesc dd, char *filename, char *bg, char *fg,
		double width, double height, Rboolean debug, Rboolean xmlHeader,
		Rboolean onefile) {
	HPGLDesc *ptd;

	if (!(ptd = (HPGLDesc *) malloc(sizeof(HPGLDesc))))
		return FALSE;

	strcpy(ptd->filename, filename);

	dd->startfill = R_GE_str2col(bg);
	dd->startcol = R_GE_str2col(fg);
	dd->startps = 10;
	dd->startlty = 0;
	dd->startfont = 1;
	dd->startgamma = 1;

	dd->activate = HPGL_Activate;
	dd->deactivate = HPGL_Deactivate;
	dd->close = HPGL_Close;
	dd->clip = HPGL_Clip;
	dd->size = HPGL_Size;
	dd->newPage = HPGL_NewPage;
	dd->line = HPGL_Line;
	dd->text = HPGL_Text;
	dd->textUTF8 = HPGL_Text; // UTF-8 support
	dd->strWidth = HPGL_StrWidth;
	dd->strWidthUTF8 = HPGL_StrWidth; // UTF-8 support
	dd->rect = HPGL_Rect;
	dd->circle = HPGL_Circle;
	dd->polygon = HPGL_Polygon;
	dd->polyline = HPGL_Polyline;
	dd->locator = HPGL_Locator;
	dd->mode = HPGL_Mode;
	dd->metricInfo = HPGL_MetricInfo;
	dd->cap = HPGL_Cap; // not implemented
	dd->raster = HPGL_Raster; // not implemented
	
	/* UTF-8 support */
	dd->wantSymbolUTF8 = 1;
	dd->hasTextUTF8 = 1;
	
	/* Screen Dimensions in Pixels */

	dd->left = 0; /* left */
	dd->right = in2dots(width);/* right */
	dd->bottom = in2dots(height); /* bottom */
	dd->top = 0; /* top */
	ptd->width = width;
	ptd->height = height;
	ptd->xmlHeader = xmlHeader;
	ptd->onefile = onefile;

	if (!HPGL_Open(dd, ptd))
		return FALSE;

	/* Base Pointsize */
	/* Nominal Character Sizes in Pixels */

	dd->cra[0] = (6.0 / 12.0) * 10.0;
	dd->cra[1] = (10.0 / 12.0) * 10.0;

	/* Character Addressing Offsets */
	/* These offsets should center a single */
	/* plotting character over the plotting point. */
	/* Pure guesswork and eyeballing ... */

	dd->xCharOffset = 0; /*0.4900;*/
	dd->yCharOffset = 0; /*0.3333;*/
	dd->yLineBias = 0; /*0.1;*/

	/* Inches per Raster Unit */
	/* We use printer points, i.e. 72.27 dots per inch : */
	dd->ipr[0] = dd->ipr[1] = 1. / DOTSperIN;

	dd->canClip = FALSE;
	dd->canHAdj = 0;
	dd->canChangeGamma = FALSE;

	ptd->lty = 1;
	ptd->pageno = 0;
	ptd->debug = debug;

	dd->deviceSpecific = (void *) ptd;
	dd->displayListOn = FALSE;
	return TRUE;
}


static pGEDevDesc RHpglDevice(char **file, char **bg, char **fg, double *width,
		double *height, int *debug, int *xmlHeader, int *onefile) {
	pGEDevDesc dd;
	pDevDesc dev;

	if (debug[0] == NA_LOGICAL)
		debug = FALSE;

	R_GE_checkVersionOrDie(R_GE_version);
	R_CheckDeviceAvailable();
	BEGIN_SUSPEND_INTERRUPTS {
		if (!(dev = (pDevDesc) Calloc(1, NewDevDesc)))
			error("unable to allocate memory for NewDevDesc");

		if (!HPGLDeviceDriver(dev, file[0], bg[0], fg[0], width[0], height[0],
				debug[0], xmlHeader[0], onefile[0])) {
			free(dev);
			error("unable to start HPGL device");
		}
		dd = GEcreateDevDesc(dev);

#if R_VERSION < R_Version(2,7,0)
		gsetVar(install(".Device"), mkString("devHPGL"), R_NilValue);
		Rf_addDevice((DevDesc*) dd);
#else
		GEaddDevice2(dd, "devHPGL");
#endif
		GEinitDisplayList(dd);
	}END_SUSPEND_INTERRUPTS;

	return (dd);
}

void do_HPGL(char **file, char **bg, char **fg, double *width, double *height,
		int *debug, int *xmlHeader, int *onefile) {
	char *vmax;

	vmax = vmaxget();

	RHpglDevice(file, bg, fg, width, height, debug, xmlHeader, onefile);

	vmaxset(vmax);
}

