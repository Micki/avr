#include "avr.h"

IMPLEMENT_APP(AvrApp);

bool AvrApp::OnInit()
{
	mainFrame *frame = new mainFrame((mainFrame*)NULL, -1, _("Hello World"), wxDefaultPosition);

	frame->Show(TRUE);
	SetTopWindow(frame);

	return true;
}