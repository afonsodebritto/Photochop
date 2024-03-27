#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <cstdint>
#include <wx/spinctrl.h>
#include "Viewer_Frame.h"
#include "My_Frame.h"

BEGIN_EVENT_TABLE(ImageViewerFrame, wxFrame)
    EVT_PAINT(ImageViewerFrame::OnPaint)
END_EVENT_TABLE()

ImageViewerFrame::ImageViewerFrame(const wxString& title, const wxSize& size, unsigned char* imageData, int imageWidth, int imageHeight)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, size)
{
    // Salve os dados da imagem em um bitmap
    wxImage image(imageWidth, imageHeight, imageData, true);
    m_imageBitmap = wxBitmap(image);

    // Define o tamanho da janela como o tamanho da imagem
    SetClientSize(wxSize(imageWidth, imageHeight));
}

void ImageViewerFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(m_imageBitmap, 0, 0);
}

void ImageViewerFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}