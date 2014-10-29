#include "mainFrame.h"

mainFrame::mainFrame(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer(0, 2, 0, 0);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	this->SetSizer(gSizer1);
	this->Layout();

	m_button1 = new wxButton(this, BUTTON_CONNECT, wxT("Connect"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer3->Add(m_button1, 0, wxALL, 5);

	m_textCtrl1 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(400, 150), wxTE_MULTILINE);
	bSizer3->Add(m_textCtrl1, 0, wxALL, 5);

	gSizer1->Add(bSizer3, 1, wxEXPAND, 5);

	// add the wxPLplot
	m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(400, 300), wxSIMPLE_BORDER | wxTAB_TRAVERSAL);
	plotwindow = new plotWindow(this, m_panel1, wxID_ANY, wxDefaultPosition, wxSize(400, 300), wxWANTS_CHARS, wxPLPLOT_BACKEND_AGG | wxPLPLOT_DRAW_TEXT);

	gSizer1->Add(m_panel1, 0, wxALL , 5);

	m_menubar1 = new wxMenuBar(0);
	m_menu1 = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("MyMenuItem")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem1);

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem(m_menu1, wxID_ANY, wxString(wxT("Exit")), wxEmptyString, wxITEM_NORMAL);
	m_menu1->Append(m_menuItem2);

	m_menubar1->Append(m_menu1, wxT("Program"));

	m_menu2 = new wxMenu();
	m_menubar1->Append(m_menu2, wxT("Connection"));

	this->SetMenuBar(m_menubar1);

	m_statusBar1 = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY);

	this->Centre(wxBOTH);

	//Serial
	m_serial = new Serial(m_statusBar1, m_textCtrl1, plotwindow);
}

void mainFrame::ButtonConnect(wxCommandEvent& event)
{
	m_serial->Connect();
	event.Skip();
}


BEGIN_EVENT_TABLE(mainFrame, wxFrame)
	EVT_BUTTON(BUTTON_CONNECT, mainFrame::ButtonConnect) // Tell the OS to run MainFrame::OnExit when
END_EVENT_TABLE() // The button is pressed