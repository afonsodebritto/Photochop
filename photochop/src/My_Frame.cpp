#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <cstdint>
#include <wx/spinctrl.h>
#include "My_Frame.h"
#include "Viewer_Frame.h"
#include "My_Canvas.h"

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
	EVT_MENU(ID_LOAD,  MyFrame::OnOpenImage)
	EVT_MENU(ID_SAVE,  MyFrame::OnSaveImage)
	EVT_MENU(ID_GRAY,  MyFrame::OnGrayScale)
	EVT_MENU(ID_INV_HOR,  MyFrame::OnInvertHorizontally)
	EVT_MENU(ID_INV_VER,  MyFrame::OnInvertVertically)
	EVT_MENU(ID_N_QUANT,  MyFrame::OnQuant)
	EVT_MENU(ID_COPY, MyFrame::OnCopyImageFromViewer)
	EVT_MENU(ID_BRIGHT, MyFrame::OnBrightnesOptions)
	EVT_MENU(ID_CONTRAST, MyFrame::OnContrastOptions)
	EVT_MENU(ID_BEST_SIZE,  MyFrame::OnBestSize)
    EVT_MENU(ID_QUIT,  MyFrame::OnQuit)
    EVT_MENU(ID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_NEG, MyFrame::OnNegative)
    EVT_MENU(ID_FILTER, MyFrame::OnFilterOptions)
	EVT_MENU(ID_LAPLACIAN, MyFrame::OnFilterOptions)
	EVT_MENU(ID_GAUSSIAN, MyFrame::OnFilterOptions)
	EVT_MENU(ID_GEN_HIGH, MyFrame::OnFilterOptions)
	EVT_MENU(ID_PHX, MyFrame::OnFilterOptions)
	EVT_MENU(ID_PHY, MyFrame::OnFilterOptions)
	EVT_MENU(ID_SHX, MyFrame::OnFilterOptions)
	EVT_MENU(ID_SHY, MyFrame::OnFilterOptions)
	EVT_MENU(ID_RCK, MyFrame::OnRotateImage)
	EVT_MENU(ID_RCCK, MyFrame::OnRotateImage)
	EVT_MENU(ID_ZI, MyFrame::OnZoomIn)
	EVT_MENU(ID_ZO, MyFrame::OnZoomOut)
	EVT_MENU(ID_PLOT, MyFrame::OnPlot)
	EVT_MENU(ID_EQ, MyFrame::OnEqualize)
	EVT_MENU(ID_EQLAB, MyFrame::OnEqualizeColor)
	EVT_MENU(ID_MATCH, MyFrame::OnMatch)
	EVT_CLOSE(MyFrame::OnClose)
END_EVENT_TABLE()



MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size),
		 m_canvas(nullptr),
		 m_imageViewer(nullptr)

{
	
	
	// Create a submenu for image options
	wxMenu* imageOptionMenu = new wxMenu();
	imageOptionMenu->Append(ID_LOAD, _T("&Open image..."));
	imageOptionMenu->Append(ID_COPY, _T("&Copy original image"));
	imageOptionMenu->Append(ID_SAVE, _T("&Save image as..."));
	imageOptionMenu->Append(ID_BEST_SIZE, _T("&Best size"));
	// Appending the submenu in the main menu
	//file_menu->AppendSubMenu(imageOptionMenu, _T("&Image Options"));
	

	// Create a submenu for the inverts
	wxMenu* move_menu = new wxMenu();
    move_menu->Append(ID_INV_HOR, _T("&Horizontally"));
    move_menu->Append(ID_INV_VER, _T("&Vertically"));
	move_menu->Append(ID_RCK, _T("&Rotate 90° Clockwise"));
    move_menu->Append(ID_RCCK, _T("&Rotate 90° Counter Clockwise"));
	move_menu->Append(ID_ZI, _T("&Zoom In"));
    move_menu->Append(ID_ZO, _T("&Zoom Out"));
	// Appending the submenu in the main menu
	//file_menu->AppendSubMenu(move_menu, _T("&Invert"));

	wxMenu* transform_menu = new wxMenu();
	transform_menu->Append(ID_GRAY, _T("&Gray scale"));
    transform_menu->Append(ID_N_QUANT, _T("&Quantisize Image"));
    transform_menu->Append(ID_BRIGHT, _T("&Adjust Brightness"));
    transform_menu->Append(ID_CONTRAST, _T("&Adjust Contrast"));
    transform_menu->Append(ID_NEG, _T("&Negative of the Image"));

	wxMenu* histogram_menu = new wxMenu();
    histogram_menu->Append(ID_PLOT, _T("&Plot Histogram"));
	histogram_menu->Append(ID_EQ, _T("&Equalize the Histogram"));
	histogram_menu->Append(ID_EQLAB, _T("&Equalize the Histogram for LAB"));
	histogram_menu->Append(ID_MATCH, _T("&Match the Histogram"));
    

	wxMenu* filterMenu = new wxMenu();
	filterMenu->Append(ID_GAUSSIAN, _T("Gaussian Filter"));
	filterMenu->Append(ID_LAPLACIAN, _T("Laplacian Filter"));
	filterMenu->Append(ID_GEN_HIGH, _T("Gen. High Pass Filter"));
	filterMenu->Append(ID_PHX, _T("Prewitt HX"));
	filterMenu->Append(ID_PHY, _T("Prewitt HY"));
	filterMenu->Append(ID_SHX, _T("Sobel HX"));
	filterMenu->Append(ID_SHY, _T("Sobel HY"));
	filterMenu->Append(ID_FILTER, _T("&Create a filter"));
	
	
	/*
	file_menu->AppendSeparator();
	file_menu->Append(ID_ABOUT, _T("&About..."));
	file_menu->AppendSeparator();
	file_menu->Append(ID_QUIT, _T("&Exit"));
	*/
	

	wxMenuBar *menuBar = new wxMenuBar();
	//menuBar->Append(file_menu, _T("&File"));
	menuBar->Append(imageOptionMenu, _T("&Image Options"));
	menuBar->Append(move_menu, _T("&Modify position/dimensios"));
	menuBar->Append(transform_menu, _T("&Transform Image"));
	menuBar->Append(filterMenu, _T("&Apply a filter"));
	menuBar->Append(histogram_menu, _T("&Histogram options"));

	SetMenuBar( menuBar );

// create the canvas that will manage the image
	m_canvas = new MyCanvas( this, -1, wxDefaultPosition, wxDefaultSize);
	m_imageLoaded = false ;
	Centre() ;
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))

{
    Close(true) ;
}

void MyFrame::OnClose(wxCloseEvent& event)

{
	delete m_canvas ;
	event.Skip() ;
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox( _T("How to \n\n- load\n- display\n- process\n- save\n\nan image with wxWidgets (2.8.7)\n\nPascal Bertolino - GIPSA-lab, Grenoble - France\npascal.bertolino@gipsa-lab.fr"),
                  _T(APP_NAME), wxOK | wxICON_INFORMATION ) ;
}

void MyFrame::OnGrayScale(wxCommandEvent& WXUNUSED(event))
{
	if (m_imageLoaded)
	    m_canvas->GrayScale(0) ;
}

void MyFrame::OnInvertHorizontally(wxCommandEvent& WXUNUSED(event))

{
	if (m_imageLoaded)
	    m_canvas->InvertHorizontally() ;
}

void MyFrame::OnInvertVertically(wxCommandEvent& WXUNUSED(event))

{
	if (m_imageLoaded)
	    m_canvas->InvertVertically() ;
}

void MyFrame::OnQuant(wxCommandEvent& WXUNUSED(event))
{
    if(m_imageLoaded)
        m_canvas->Options(0);
}

void MyFrame::OnOpenImage(wxCommandEvent& WXUNUSED(event))
{
    filename = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*") );

	if(m_imageViewer)
	{
		m_imageViewer->Close();
		delete m_imageViewer;
		m_imageViewer = nullptr;
	}

    if (!filename.empty())
    {
        m_canvas->LoadImage(filename, 0);
        m_imageLoaded = true;

        int imageWidth = m_canvas->GetImageWidth();
        int imageHeight = m_canvas->GetImageHeight();
        const unsigned char* imageData = m_canvas->GetImageData();

        m_imageViewer = new ImageViewerFrame("Original Image", wxSize(imageWidth, imageHeight), const_cast<unsigned char*>(imageData), imageWidth, imageHeight);
		wxIcon iconImageViewer;
		if (iconImageViewer.LoadFile("icon.png", wxBITMAP_TYPE_PNG))
		{
		m_imageViewer->SetIcon(iconImageViewer);
		}
        m_imageViewer->Show(true);
    }
}

void MyFrame::OnSaveImage(wxCommandEvent & WXUNUSED(event))
{
//	char str[128] = "" ; // proposed file name

	if (!m_imageLoaded)
		return ;

	wxString filename = wxFileSelector(_T("Save image as"),_T(""),_T(""),_T("*.bmp"), _T("BMP files (*.bmp)|*.bmp|GIF files (*gif)|*.gif|JPEG files (*jpg)|*.jpg|PNG files (*png)|*.png|TIFF files (*tif)|*.tif|XPM files (*xpm)|*.xpm|All files (*.*)|*.*"));
	if ( !filename.empty() )
		m_canvas->SaveImage(filename) ;
}

void MyFrame::OnBestSize(wxCommandEvent& WXUNUSED(event))
{
    m_canvas->BestSize() ;
}

void MyFrame::OnQuantButtonClick(wxCommandEvent& event)
{
    m_canvas->GrayQuantisize();
}

void MyFrame::OnCopyImageFromViewer(wxCommandEvent & WXUNUSED(event))
{
	if(!m_imageLoaded)
	{
		wxMessageBox("No image opened yet");
		return;
	}
	else
    {
        m_canvas->LoadImage(filename, 0);

        int imageWidth = m_canvas->GetImageWidth();
        int imageHeight = m_canvas->GetImageHeight();
        const unsigned char* imageData = m_canvas->GetImageData();
    }
}

void MyFrame::OnBrightnesOptions(wxCommandEvent& WXUNUSED(event))
{
    if(m_imageLoaded)
        m_canvas->Options(1);
}

void MyFrame::OnContrastOptions(wxCommandEvent& WXUNUSED(event))
{
    if(m_imageLoaded)
        m_canvas->Options(2);
}

void MyFrame::OnNegative(wxCommandEvent& WXUNUSED(event))
{
	if(m_imageLoaded)
        m_canvas->Negative();
}

void MyFrame::OnFilterOptions(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		int filterId = event.GetId();

    	switch (filterId) {
			case ID_GAUSSIAN:
			std::cout << filterId << std::endl;
				m_canvas->Options(30);
				break;
			case ID_LAPLACIAN:
				m_canvas->Options(31);
				break;
			case ID_GEN_HIGH:
				m_canvas->Options(32);
				break;
			case ID_PHX:
				m_canvas->Options(33);
				break;
			case ID_PHY:
				m_canvas->Options(34);
				break;
			case ID_SHX:
				m_canvas->Options(35);
				break;
			case ID_SHY:
				m_canvas->Options(36);
				break;
			default:
				m_canvas->Options(3);
				break;
    	}
		
	}
    
}

void MyFrame::OnRotateImage(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		int filterId = event.GetId();
		switch(filterId)
		{
			case ID_RCK:
				m_canvas->RotateClockwise();
				break;
			case ID_RCCK:
				m_canvas->RotateCounterClockwise();
				break;
		}
			
	}
}

void MyFrame::OnZoomIn(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		m_canvas->ZoomIn();
	}
}

void MyFrame::OnZoomOut(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		m_canvas->Options(4);
	}
}

void MyFrame::OnPlot(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		m_canvas->PlotHistogram();
	}
}

void MyFrame::OnEqualize(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		m_canvas->EqualizeHistogram();
	}
}

void MyFrame::OnMatch(wxCommandEvent& event)
{
	filenameTarget = wxFileSelector(_T("Select file"),_T(""),_T(""),_T(""), _T("All files (*.*)|*.*") );

	if(m_imageViewerTarget)
	{
		m_imageViewerTarget->Close();
		delete m_imageViewerTarget;
		m_imageViewerTarget = nullptr;
	}

    if (!filename.empty())
    {
        m_canvas->LoadImage(filenameTarget, 1);

        int imageWidth = m_canvas->GetTargetWidth();
        int imageHeight = m_canvas->GetTargetHeight();
        const unsigned char* imageData = m_canvas->GetTargetImageData();
		m_canvas->GrayScale(1);

        m_imageViewerTarget = new ImageViewerFrame("Target Image", wxSize(imageWidth, imageHeight), const_cast<unsigned char*>(imageData), imageWidth, imageHeight);
		wxIcon iconImageViewer;
		if (iconImageViewer.LoadFile("icon.png", wxBITMAP_TYPE_PNG))
		{
		m_imageViewerTarget->SetIcon(iconImageViewer);
		}
        m_imageViewerTarget->Show(true);
    }

	m_canvas->MatchHistogram();
}

void MyFrame::OnEqualizeColor(wxCommandEvent& event)
{
	if(m_imageLoaded)
	{
		m_canvas->EqualizeHistogramColor();
	}
}