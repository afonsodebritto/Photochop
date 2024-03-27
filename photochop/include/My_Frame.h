#ifndef IMAGE_MY_FRAME_H
#define IMAGE_MY_FRAME_H

#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <cstdint>
#include <wx/spinctrl.h>
#include "My_Canvas.h"
#include "Viewer_Frame.h"

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnQuantButtonClick(wxCommandEvent& event);
	
protected:
	void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	void OnOpenImage(wxCommandEvent& WXUNUSED(event) ) ;
	void OnSaveImage(wxCommandEvent & WXUNUSED(event)) ;
	void OnGrayScale(wxCommandEvent& WXUNUSED(event) ) ;
	void OnInvertHorizontally(wxCommandEvent& WXUNUSED(event) ) ;
	void OnInvertVertically(wxCommandEvent& WXUNUSED(event) ) ;
	void OnQuant(wxCommandEvent& WXUNUSED(event) ) ;
	void OnClose(wxCloseEvent& event) ;
	void OnBrightnesOptions(wxCommandEvent& WXUNUSED(event)) ;
	void OnContrastOptions(wxCommandEvent& WXUNUSED(event)) ;
	void OnFilterOptions(wxCommandEvent& WXUNUSED(event)) ;
	void OnNegative(wxCommandEvent& WXUNUSED(event) ) ;
	void OnBestSize(wxCommandEvent& WXUNUSED(event)) ;
	void OnCopyImageFromViewer(wxCommandEvent& WXUNUSED(event)) ;
	void OnRotateImage(wxCommandEvent& WXUNUSED(event)) ;
	void OnZoomIn(wxCommandEvent& WXUNUSED(event)) ;
	void OnZoomOut(wxCommandEvent& WXUNUSED(event)) ;
	void OnHistogram(wxCommandEvent& WXUNUSED(event)) ;
	void OnPlot(wxCommandEvent& WXUNUSED(event)) ;
	void OnEqualize(wxCommandEvent& WXUNUSED(event)) ;
	void OnEqualizeColor(wxCommandEvent& WXUNUSED(event)) ;
	void OnMatch(wxCommandEvent& WXUNUSED(event)) ;

    MyCanvas *m_canvas; // The canvas inside the main frame
	wxString filename; // Name of the image file
	ImageViewerFrame* m_imageViewer; // The image viewer that shows the original image
	wxString filenameTarget; // Name of the image file target in the histogram matching
	ImageViewerFrame* m_imageViewerTarget; // The image viewer that shows target image on histogram matching
	bool m_imageLoaded ;
    DECLARE_EVENT_TABLE()
};

#endif