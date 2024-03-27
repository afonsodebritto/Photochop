#ifndef MY_CANVAS_H
#define MY_CANVAS_H

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
#include "Viewer_Frame.h"

#define APP_NAME "Photochopp"

// Every option available
enum
{
    ID_QUIT = 1,
    ID_ABOUT,
	ID_LOAD,
	ID_SAVE,
	ID_GRAY,
    ID_INV_HOR,
    ID_INV_VER,
    ID_COPY,
    ID_BRIGHT,
	ID_BEST_SIZE,
    ID_N_QUANT,
    ID_CONTRAST,
    ID_FILTER,
    ID_NEG,
    ID_GAUSSIAN,
    ID_LAPLACIAN,
    ID_GEN_HIGH,
    ID_PHX,
    ID_PHY,
    ID_SHX,
    ID_SHY,
    // Rotate clockwise
    ID_RCK,
    //Rotate counter clockwise
    ID_RCCK,
    // Zoom in the image
    ID_ZI,
    // Zoom out the image
    ID_ZO,
    // Computes and plots the histogram
    ID_PLOT,
    // Equalize the histogram
    ID_EQ,
    // Matching of two histograms
    ID_MATCH,
    ID_EQLAB
};

class MyCanvas: public wxPanel
{
public:
    MyCanvas( wxWindow *parent, wxWindowID, const wxPoint &pos, const wxSize &size ) ;
    ~MyCanvas() ;
	void LoadImage(wxString fileName, int option) ;
	void SaveImage(wxString fileName) ;
	void GrayScale(int option) ;
    void InvertHorizontally();
    void InvertVertically();
    void OnQuantButtonClick(wxCommandEvent& event);
    void GrayQuantisize();
    void OnBrightnessButtonClick(wxCommandEvent& event);
    void AdjustBrightness();
    void Options(int option);
    void OptionButton(wxCommandEvent& event, int option);
    void OnContrastButtonClick(wxCommandEvent& event);
    void AdjustContrast();
    void ApplyFilter(int filterOption);
    void RotateClockwise();
    void RotateCounterClockwise();
    void ZoomIn();
    void ZoomOut();
    void Negative();
    void PlotHistogram();
    void EqualizeHistogram();
    void EqualizeHistogramColor();
    void MatchHistogram();
	void BestSize() ;
    int GetImageWidth() const { return m_imageWidth; }
    int GetImageHeight() const { return m_imageHeight; }
    const unsigned char* GetImageData() const { return m_myImage; }
    int GetTargetWidth() const { return targetWidth; }
    int GetTargetHeight() const { return targetHeight; }
    const unsigned char* GetTargetImageData() const { return m_myImageTarget; }

private:
	int m_imageWidth ;
	int m_imageHeight ;
    wxBitmap m_imageBitmap ;	// used to display the image
	wxImage *m_imageRGB ;		// used to load the image
    unsigned char* m_myImage ;	// used to process the image

    void OnPaint(wxPaintEvent &event) ;

    DECLARE_EVENT_TABLE()

    // Used in the options funcion:
    wxTextCtrl* m_spinCtrl;
    // Used for the zoom out function
    wxTextCtrl* m_spinCtrl2;
    wxButton* m_button;
    // Used to apply filters to the image
    // Size of the kernel = 3x3
    wxTextCtrl* kernel[3][3];
    ImageViewerFrame* m_imageViewer; // The image viewer that displays the kernel

    int histogram[256];
    int plotHistogram;

    int targetWidth;
    int targetHeight;
    wxImage *m_imageTargetRGB ;		// Used to load the targer image of histogram matching
    unsigned char* m_myImageTarget ;	// Used to process the target image of histogram matching

    // Temporary image
    unsigned char* m_copyImage;
    // Default filters
    enum Filter
    {
        GAUSSIAN, LAPLACIAN, GEN_HIGH, PHX, PHY, SHX, SHY
    };
};

#endif