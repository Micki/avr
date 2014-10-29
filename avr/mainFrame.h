#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

#include "serial.h"

class mainFrame : public wxFrame
{
private:

protected:
	wxButton* m_button1;
	wxTextCtrl* m_textCtrl1;
	wxMenuBar* m_menubar1;
	wxMenu* m_menu1;
	wxMenu* m_menu2;
	wxStatusBar* m_statusBar1;

	wxPanel* m_panel1;

	Serial* m_serial;

public:
	mainFrame(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(900, 800), long style = wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER | wxTAB_TRAVERSAL);

	virtual void ButtonConnect(wxCommandEvent& event);

	plotWindow* plotwindow;

	DECLARE_EVENT_TABLE()
};

enum
{
	BUTTON_CONNECT = wxID_HIGHEST + 1 // declares an id which will be used to call our button
};