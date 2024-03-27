#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <cstdint>
#include <wx/spinctrl.h>
#include "My_Canvas.h"
#include "My_Frame.h"
#include "Viewer_Frame.h"

class MyApp: public wxApp
{
    virtual bool OnInit() ;
};

IMPLEMENT_APP(MyApp) // macro that contains the main() function

bool MyApp::OnInit()
{
//support all available image formats
	wxInitAllImageHandlers() ;

    MyFrame *frame = new MyFrame(_T(APP_NAME), wxDefaultPosition, wxSize(400,300)) ;

    // Seeting an icon to the main window
    wxIcon icon;
	if (icon.LoadFile("icon.png", wxBITMAP_TYPE_PNG))
	{
		frame->SetIcon(icon);
	}
    frame->Show(true) ;
    SetTopWindow(frame) ;
    return true ;
}
