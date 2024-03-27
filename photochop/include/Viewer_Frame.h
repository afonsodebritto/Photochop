#ifndef IMAGE_VIEWER_FRAME_H
#define IMAGE_VIEWER_FRAME_H

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

#define APP_NAME "Photochopp"

class ImageViewerFrame : public wxFrame {
public:
    ImageViewerFrame(const wxString& title, const wxSize& size, unsigned char* imageData, int imageWidth, int imageHeight);

private:
    wxBitmap m_imageBitmap;

    void OnPaint(wxPaintEvent& event);
    void OnClose(wxCloseEvent& event);

    DECLARE_EVENT_TABLE()
};

#endif // IMAGE_VIEWER_FRAME_H