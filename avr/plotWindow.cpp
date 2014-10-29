#include "plotWindow.h"

plotWindow::plotWindow(wxFrame* frame, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, int pl_style) 
	: wxPLplotwindow(parent, id, pos, size, style, pl_style)
{
	mframe = frame;
}

void plotWindow::Plot()
{
	wxPLplotstream* pls = this->GetStream();

	const size_t  np = 500;
	PLFLT         x[np], y[np];
	PLFLT         xmin, xmax;
	PLFLT         ymin = 1, ymax = 20;

	xmin = -2.0;
	xmax = 10.0;
	for (size_t i = 0; i < np; i++)
	{
		x[i] = (xmax - xmin) * i / np + xmin;
		y[i] = 1.0;
		if (x[i] != 0.0)
			y[i] = sin(x[i]) / x[i];
		ymin = MIN(ymin, y[i]);
		ymax = MAX(ymax, y[i]);
	}

	pls->adv(0);

	pls->scol0(15, 255, 255, 255);
	pls->scol0(0, 0, 0, 0);

	pls->col0(1);
	pls->env(xmin, xmax, ymin, ymax, 0, 0);
	pls->col0(2);
	pls->lab("x", "y", "sin(x)/x");

	pls->col0(3);
	pls->width(2);
	pls->line(np, x, y);

	this->RenewPlot();
}