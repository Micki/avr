#include <plplot/wxPLplotwindow.h>

#define MAX( a, b )    ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b )    ( ( a ) < ( b ) ? ( a ) : ( b ) )

class plotWindow : public wxPLplotwindow
{
public:
	plotWindow(wxFrame* frame, wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = 0, int pl_style = wxPLPLOT_NONE);
	void plotWindow::Plot();
private:
	wxFrame* mframe;
};