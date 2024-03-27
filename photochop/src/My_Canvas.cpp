#include <wx/image.h>
#include <wx/file.h>
#include <wx/bitmap.h>
#include <cstdint>
#include <wx/event.h>
#include <wx/spinctrl.h>
#include <string>
#include "My_Canvas.h"
#include "My_Frame.h"
#include "Viewer_Frame.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>

#include <windows.h>


BEGIN_EVENT_TABLE(MyCanvas, wxPanel)
	EVT_PAINT(MyCanvas::OnPaint)
END_EVENT_TABLE()

MyCanvas::MyCanvas( wxWindow *parent, wxWindowID id,
                    const wxPoint &pos, const wxSize &size )
        : wxPanel( parent, id, pos, size, wxSUNKEN_BORDER),
        m_imageViewer(nullptr),
        m_copyImage(nullptr)
{
    m_myImage=NULL;
	m_imageRGB = NULL ;
}

MyCanvas::~MyCanvas()
{
	if (m_myImage)
		free(m_myImage) ;
	if (m_imageRGB)
		delete m_imageRGB ;
}

/*
    Options:
    - 0: open a new image
    - 1: open a target image for histogram matching
*/
void MyCanvas::LoadImage(wxString fileName, int option)
{
    if(option == 0)
    {
        if (m_myImage)
		    free (m_myImage) ;
        if (m_imageRGB)
            delete m_imageRGB ;

        // Open image dialog box
        m_imageRGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1); // ANY => can load many image formats
        m_imageBitmap = wxBitmap(*m_imageRGB, -1); // ...to get the corresponding bitmap

        m_imageWidth = m_imageRGB->GetWidth() ;
        m_imageHeight = m_imageRGB->GetHeight() ;

        m_myImage = (unsigned char*)malloc(m_imageWidth * m_imageHeight * 3) ;
        memcpy(m_myImage, m_imageRGB->GetData(), m_imageWidth * m_imageHeight * 3) ;

        // update GUI size
        SetSize(m_imageWidth, m_imageHeight) ;
        GetParent()->SetClientSize(GetSize()) ;

        // update display
        Refresh(false) ;
    }
    else if(option == 1)
    {
        if(m_myImageTarget)
            free(m_myImageTarget);
        if(m_imageTargetRGB)
            delete m_imageTargetRGB;

        // Open image dialog box
        m_imageTargetRGB = new wxImage(fileName, wxBITMAP_TYPE_ANY, -1); // ANY => can load many image formats

        targetWidth = m_imageTargetRGB->GetWidth();
        targetHeight = m_imageTargetRGB->GetHeight();

        m_myImageTarget = (unsigned char*)malloc(targetWidth * targetHeight * 3) ;
        memcpy(m_myImageTarget, m_imageTargetRGB->GetData(), targetWidth * targetHeight * 3) ;
    }
	
}

void MyCanvas::SaveImage(wxString fileName)
{
	bool b ;

	wxImage* tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
	b = tempImage->SaveFile(fileName) ;
	delete(tempImage) ;		// buffer not needed any more

	if(!b)
		wxMessageBox(wxT("A problem occured during saving"));
}

/*
    Options:
    - 0: Original image
    - 1: Target image of histogram matching
*/
void MyCanvas::GrayScale(int option)
// Function that turns a RGB picture into a Gray Scale one
{
    if(option == 0)
    {
        long int size = m_imageWidth*m_imageHeight*3 ;
        int i;

        for(i = 0; i < size; i += 3)
        {
            float gray = 0.299*m_myImage[i] + 0.587*m_myImage[i+1] + 0.114*m_myImage[i+2];
            memset(m_myImage + i, gray, 3);
        }

        Refresh(false); // update display
    }
    else if(option == 1)
    {
        long int size = targetWidth * targetHeight * 3 ;
        int i;

        for(i = 0; i < size; i += 3)
        {
            float gray = 0.299*m_myImageTarget[i] + 0.587*m_myImageTarget[i+1] + 0.114*m_myImageTarget[i+2];
            memset(m_myImageTarget + i, gray, 3);
        }
    }
	
}

// Function that inverts an image horizontally
void MyCanvas::InvertHorizontally()
{
    uint8_t temp[4];
    uint8_t* pixel1;
    uint8_t* pixel2;
    int row, column;

    for(row = 0; row < m_imageHeight; ++row)
    {
        for(column = 0; column < m_imageWidth/2; ++column)
        {
            pixel1 = &m_myImage[(column + row*m_imageWidth) * 3];
            pixel2 = &m_myImage[((m_imageWidth - 1 - column) + row * m_imageWidth) * 3];
            memcpy(temp, pixel1, 3);
            memcpy(pixel1, pixel2, 3);
            memcpy(pixel2, temp, 3);
        }
    }

    Refresh(false);
}

void MyCanvas::InvertVertically()
{
    int i;
    int column_offset, line_size;
    line_size = m_imageWidth* 3;
    uint8_t* temp_line = new uint8_t [m_imageWidth* 3];
    uint8_t *original_line, *copy_line;
    long int size = m_imageWidth*m_imageHeight*3 ;

    for(i = 0; i < (size/2); i += line_size)
    {
        column_offset = size - line_size - i;
        original_line = &m_myImage[i];
        copy_line = &m_myImage[column_offset];
        
        memcpy(temp_line, original_line, line_size);
        memcpy(original_line, copy_line, line_size);
        memcpy(copy_line, temp_line, line_size);
    }

    delete temp_line;
    Refresh(false);
}

void MyCanvas::BestSize()
{
	SetSize(m_imageWidth, m_imageHeight) ;	// ideal size for canvas
	GetParent()->SetClientSize(GetSize());	// force the main frame to show the whole canvas
}

void MyCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
// update the main window content
{
    wxImage* tempImage;  // the bridge between my image buffer and the bitmap to display

	wxPaintDC dc(this);

    if(m_myImage)
	{
		tempImage = new wxImage(m_imageWidth, m_imageHeight, m_myImage, true); // lend my image buffer...
		m_imageBitmap = wxBitmap(*tempImage, -1); // ...to get the corresponding bitmap
		delete(tempImage) ;		// buffer not needed any more
		dc.DrawBitmap(m_imageBitmap, 0, 0) ;
	}

    if(plotHistogram)
    {
        // Configuring the draw
        dc.SetBrush(*wxBLUE_BRUSH);
        dc.SetPen(*wxBLACK_PEN);

        // Size of the bar
        int barWidth = 2;

        // Draw the histogram
        int numBars = 256;
        for (int i = 0; i < numBars; i++)
        {
            int barHeight = histogram[i];
            int x = i * (barWidth + 1); // Space between the bars
            int y = 400 - barHeight;

            dc.DrawRectangle(x, y, barWidth, barHeight);
        }
        plotHistogram = 0;
    }
}

/*
    Options:
    - 0: Quantization
    - 1: Brightness
    - 2: Contrast
    - 3: Filter
    - 3x: Default filter
    - 4: Zoom out
*/
void MyCanvas::Options(int option) {
    // Dimension of the spin and the button
    int spinWidth = 100;
    int spinHeight = 25;
    int buttonWidth = 100;
    int buttonHeight = 25;

    // Texts
    std::string spinText;
    std::string spinText2;
    std::string buttonText;

    // Limits
    int lowerLimit, upperLimit;

    // Creating the spin control for the other options
    if(option >= 30)
    {
        ApplyFilter(option % 30);
        return;
    }

    if(option != 3)
    {
        // Creating a solid white image
        wxImage solidImage(300, 200);
        solidImage.SetRGB(wxRect(0, 0, 300, 200), 255, 255, 255);
        // Converting it to an array of bytes
        unsigned char* imageData = solidImage.GetData();

        m_imageViewer = new ImageViewerFrame("Options", wxSize(300, 200), imageData, 300, 200);
    }
        
    if(option == 0) 
    {
        spinText = "Number of bins";
        buttonText = "Quantize";
        lowerLimit = 1;
        upperLimit = 255;
    }
    else if(option == 1)
    {
        spinText = "Level of Brightness";
        buttonText = "Adjust";
        lowerLimit = -255;
        upperLimit = 255;
    }
    else if(option == 2)
    {
        spinText = "Level of contrast";
        buttonText = "Adjust";
        lowerLimit = 0;
        upperLimit = 255;
    }
    else if (option == 3) {
    // Creating a solid white image
    wxImage solidImage(400, 300);
    solidImage.SetRGB(wxRect(0, 0, 400, 300), 255, 255, 255);
    // Converting it to an array of bytes
    unsigned char* imageData = solidImage.GetData();

    m_imageViewer = new ImageViewerFrame("Kernel", wxSize(400, 300), imageData, 400, 300);

    int numRows = sizeof(kernel) / sizeof(kernel[0]);
    int numCols = sizeof(kernel[0]) / sizeof(kernel[0][0]);

    // Determinar a largura total necessária para os controles de giro
    int totalSpinWidth = numCols * spinWidth + (numCols - 1) * 10; // Considerando uma lacuna de 10 pixels entre os controles

    // Centralizar horizontalmente os controles de giro
    int startX = (m_imageViewer->GetClientSize().GetWidth() - totalSpinWidth) / 2;

    int i, j;
    for (i = 0; i < numRows; i++) {
        for (j = 0; j < numCols; j++) {
            kernel[i][j] = new wxTextCtrl(m_imageViewer, wxID_ANY, std::to_string(i + j), wxDefaultPosition, wxSize(spinWidth, spinHeight));
            kernel[i][j]->Show();

            // Centralizar os controles de giro horizontalmente
            int spinX = startX + j * (spinWidth + 10);
            int spinY = 50 * (i + 1) + 10 * i; // Adicionando uma margem vertical entre as linhas
            kernel[i][j]->SetPosition(wxPoint(spinX, spinY));
        }
    }

    // Calcular a altura total com base na posição Y do último controle de giro
    int lastSpinY = 50 * numRows + 10 * (numRows - 1); // Posição Y do último controle
    int totalHeight = lastSpinY + spinHeight + 10; // Adicione uma margem de 10 pixels

    // Centralizar o botão horizontalmente e verticalmente
    int buttonX = (m_imageViewer->GetClientSize().GetWidth() - totalSpinWidth) / 2;
    int buttonY = 50 * (i + 1) + 10 * i;
    m_button = new wxButton(m_imageViewer, wxID_ANY, "Apply", wxDefaultPosition, wxSize(totalSpinWidth, buttonHeight));
    m_button->Show();
    m_button->SetPosition(wxPoint(buttonX, buttonY));

    wxIcon iconImageViewer;
    if (iconImageViewer.LoadFile("icon.png", wxBITMAP_TYPE_PNG)) {
        m_imageViewer->SetIcon(iconImageViewer);
    }
    m_imageViewer->Show(true);
    }
    else if(option == 4)
    {
        spinText = "Sx";
        buttonText = "Zoom out";
        lowerLimit = 0;
        upperLimit = 255;
        m_spinCtrl2 = new wxTextCtrl(m_imageViewer, wxID_ANY, "Sy", wxDefaultPosition, wxSize(spinWidth, spinHeight));
    }

    if(option != 3) {
    m_spinCtrl = new wxTextCtrl(m_imageViewer, wxID_ANY, spinText, wxDefaultPosition, wxSize(spinWidth, spinHeight));
    m_spinCtrl->Show();
    // Getting the size of the spin control
    wxSize spinSize = m_spinCtrl->GetSize();
    
    // Positioning the spin control
    int spinX = (m_imageViewer->GetClientSize().GetWidth() - spinSize.x) / 2; // Centering the spin control horizontally
    int spinY = 50; // Y coordinate of the spin control
    m_spinCtrl->SetPosition(wxPoint(spinX, spinY));
    if(option == 4)
    {
        spinY = spinHeight + 60;
        m_spinCtrl2->SetPosition(wxPoint(spinX, spinY));
    }
    
    m_button = new wxButton(m_imageViewer, wxID_ANY, buttonText, wxDefaultPosition, wxSize(buttonWidth, buttonHeight));

    m_button->Show();

    // Getting the size of the button
    wxSize buttonSize = m_button->GetSize();

    // Positioning the button below the wxSpinCtrl
    int buttonX = (m_imageViewer->GetClientSize().GetWidth() - buttonSize.x) / 2; // Centering the button horizontally
    int buttonY = spinY + spinSize.y + 10; // 10 pixels below the wxSpinCtrl
    m_button->SetPosition(wxPoint(buttonX, buttonY));

    m_imageViewer->Show(true);
    }
    
    // Calls the function to each option
    if(option < 30)
    {
        m_button->Bind(wxEVT_BUTTON, [this, option](wxCommandEvent& event) { OptionButton(event, option); });
    }

    Refresh(false);
}

/*
    Options:
    - 0: Quantization
    - 1: Brightness
    - 2: Contrast
    - 3: Filter
*/

void MyCanvas::OptionButton(wxCommandEvent& event, int option)
{
    switch(option)
    {
        case 0:
            GrayQuantisize();
            break;
        case 1:
            AdjustBrightness();
            break;
        case 2:
            AdjustContrast();
            break;
        case 3:
            ApplyFilter(-1);
            break;
        case 4:
            ZoomOut();
            break;
        default:
            wxMessageBox("No such option");
            break;
    }
    
}

void MyCanvas::GrayQuantisize()
{
    int n;
    wxString valueString = m_spinCtrl->GetValue();
    long nLong;
    if (valueString.ToLong(&nLong)) 
    {
        n = static_cast<int>(nLong);
        n = std::max(1, std::min(255, n));
    }
    uint8_t t1;
    uint8_t t2;
    uint8_t tam_int;
    int i, j;
    float tb;
    // Interval of a bin
    float beg_bin, end_bin;
    long int size = m_imageWidth*m_imageHeight*3 ;

    // Set t1 to the highest value possible
    t1 = 255;
    // Set t2 to the lowest value possible
    t2 = 0;

    // Find the highest and the lowest tones
    for(i = 0; i < size; i++)
    {
        if(m_myImage[i] < t1)
            t1 = m_myImage[i];
        if(m_myImage[i] > t2)
            t2 = m_myImage[i];
    }

    tam_int = t2 - t1 + 1;


    if(n >= tam_int)
    {
        wxMessageBox("There are enough tones already.");
    }
    else
    {
        tb = tam_int / n;

        for(i = 1; i <= n; i++)
        {
            // Defines the bounds of the bean
            beg_bin = t1 - 0.5 + (i - 1)*tb;
            end_bin = t1 - 0.5 + i*tb;
            for(j = 0; j < size; j++)
            {
                // Checks every pixel and if it goes in that bin
                if(m_myImage[j] >= beg_bin && m_myImage[j] < end_bin)
                    m_myImage[j] = int((beg_bin + end_bin)/2);
            }
        }
    }

    Refresh(false);

}

void MyCanvas::AdjustBrightness()
{
    int brightnessFactor;
    int i;
    // Gets the adjust factor
    wxString valueString = m_spinCtrl->GetValue();
    long brightnessFactorLong;
    if (valueString.ToLong(&brightnessFactorLong)) 
    {
        brightnessFactor = static_cast<int>(brightnessFactorLong);
        brightnessFactor = std::max(-255, std::min(255, brightnessFactor));
    }

    // Adjust each pixel
    for(i = 0; i < m_imageWidth * m_imageHeight * 3; i++)
    {
        if((m_myImage[i])+brightnessFactor < 0) {
            m_myImage[i] = 0;
        }
        else if((m_myImage[i])+brightnessFactor > 255) {
            m_myImage[i] = 255;
        }
        else {
            m_myImage[i] = (m_myImage[i])+brightnessFactor;
        }
    }

    Refresh(false);

}

void MyCanvas::AdjustContrast()
{
    int i;
    int contrastFactor;
    // Gets the adjust factor
    wxString valueString = m_spinCtrl->GetValue();
    long contrastFactorLong;
    if (valueString.ToLong(&contrastFactorLong)) 
    {
        contrastFactor = static_cast<int>(contrastFactorLong);
        contrastFactor = std::max(0, std::min(255, contrastFactor));
    }

    // Adjust each pixel
    for(i = 0; i < m_imageWidth * m_imageHeight * 3; i++)
    {
        if((m_myImage[i])*contrastFactor < 0) {
            m_myImage[i] = 0;
        }
        else if((m_myImage[i])*contrastFactor > 255) {
            m_myImage[i] = 255;
        }
        else {
            m_myImage[i] = (m_myImage[i])*contrastFactor;
        }
    }

    Refresh(false);

}

void MyCanvas::ApplyFilter(int filterOption)
{
    const int rows = 3; 
    const int cols = 3;
    float filter[rows][cols];
    // Gets the kernel values
    if(filterOption == -1)
    {
        for(int i = 0; i < rows; i++) 
        {
            for(int j = 0; j < cols; j++) 
            {
                wxString valueString = kernel[i][j]->GetValue();
                double value;
                if (valueString.ToDouble(&value)) 
                {
                    // Converts the value to a float
                    float floatValue = static_cast<float>(value);
                    filter[i][j] = floatValue;
                } else 
                {
                    // If it can't convert just assigns zero
                    filter[i][j] = 0.0;
                }
            }
        }
    }
    else
    {
        // Casts the int to the enum type
        Filter filterOptionEnum = static_cast<Filter>(filterOption);
        switch (filterOptionEnum) {
        case GAUSSIAN:
            filter[0][0] = 0.0625; filter[0][1] = 0.125; filter[0][2] = 0.0625;
            filter[1][0] = 0.125; filter[1][1] = 0.25; filter[1][2] = 0.125;
            filter[2][0] = 0.0625; filter[2][1] = 0.125; filter[2][2] = 0.0625;
            break;
        case LAPLACIAN:
            filter[0][0] = 0; filter[0][1] = -1; filter[0][2] = 0;
            filter[1][0] = -1; filter[1][1] = 4; filter[1][2] = -1;
            filter[2][0] = 0; filter[2][1] = -1; filter[2][2] = 0;
            break;
        case GEN_HIGH:
            filter[0][0] = -1; filter[0][1] = -1; filter[0][2] = -1;
            filter[1][0] = -1; filter[1][1] = 8; filter[1][2] = -1;
            filter[2][0] = -1; filter[2][1] = -1; filter[2][2] = -1;
            break;
        case PHX:
            filter[0][0] = -1; filter[0][1] = 0; filter[0][2] = 1;
            filter[1][0] = -1; filter[1][1] = 0; filter[1][2] = 1;
            filter[2][0] = -1; filter[2][1] = 0; filter[2][2] = 1;
            break;
        case PHY:
            filter[0][0] = -1; filter[0][1] = -1; filter[0][2] = -1;
            filter[1][0] = 0; filter[1][1] = 0; filter[1][2] = 0;
            filter[2][0] = 1; filter[2][1] = 1; filter[2][2] = 1;
            break;
        case SHX:
            filter[0][0] = -1; filter[0][1] = 0; filter[0][2] = 1;
            filter[1][0] = -2; filter[1][1] = 0; filter[1][2] = 2;
            filter[2][0] = -1; filter[2][1] = 0; filter[2][2] = 1;
            break;
        case SHY:
            filter[0][0] = -1; filter[0][1] = -2; filter[0][2] = -1;
            filter[1][0] = 0; filter[1][1] = 0; filter[1][2] = 0;
            filter[2][0] = 1; filter[2][1] = 2; filter[2][2] = 1;
            break;
        default:
            wxString message = "No such filter: " + wxString::Format("%d", filterOption);
            wxMessageBox(message);
            break;
        }
    }
    
    int y, x, l, k;
    int posImage;
    int channels = 3;
    float sumRed, sumGreen, sumBlue;
    
    std::vector<unsigned char> m_copyImage(m_myImage, m_myImage + m_imageWidth * m_imageHeight * 3);

    for (int y = 1; y < m_imageHeight - 1; y++)
    {
        for (int x = 1; x < m_imageWidth - 1; x++)
        {
            float sumRed = 0, sumGreen = 0, sumBlue = 0;

            // Applying the filter
            for (int k = -rows / 2; k <= rows / 2; k++)
            {
                for (int l = -cols / 2; l <= cols / 2; l++)
                {
                    int posY = y + k;
                    int posX = x + l;

                    if (posY >= 0 && posY < m_imageHeight && posX >= 0 && posX < m_imageWidth)
                    {
                        int posImage = (posY * m_imageWidth + posX) * 3;
                        sumRed += m_copyImage[posImage] * filter[k + rows / 2][l + cols / 2];
                        sumGreen += m_copyImage[posImage + 1] * filter[k + rows / 2][l + cols / 2];
                        sumBlue += m_copyImage[posImage + 2] * filter[k + rows / 2][l + cols / 2];
                    }
                }
            }

            if (filterOption == PHX || filterOption == PHY || filterOption == SHX || filterOption == SHY)
            {
                sumRed += 127;
                sumGreen += 127;
                sumBlue += 127;
            }

            // Clamps the result
            sumRed = std::max(std::min(sumRed, 255.0f), 0.0f);
            sumGreen = std::max(std::min(sumGreen, 255.0f), 0.0f);
            sumBlue = std::max(std::min(sumBlue, 255.0f), 0.0f);

            // Updates the pixel on the original image
            int pos = (y * m_imageWidth + x) * 3;
            m_myImage[pos] = static_cast<unsigned char>(sumRed);
            m_myImage[pos + 1] = static_cast<unsigned char>(sumGreen);
            m_myImage[pos + 2] = static_cast<unsigned char>(sumBlue);
        }
    }

    Refresh(false);
}


void MyCanvas::Negative()
{
    int i;
    for(i = 0; i < m_imageWidth * m_imageHeight * 3; i++)
    {
        m_myImage[i] = 255 - m_myImage[i];
    }

    Refresh(false);
}

void MyCanvas::RotateClockwise()
{
    int i, j;
    int posImage, posRotate;
    // Original image is NxM
    int n, m;
    n = m_imageHeight;
    m = m_imageWidth;

    unsigned char* m_myNewImage = (unsigned char*)malloc(m_imageWidth * m_imageHeight * 3);

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            // rotatedImage[j][n - i - 1] = originalImage[i][j];
            posImage = i * m * 3 + j*3;
            posRotate = (j * n * 3) + (n - i - 1) * 3;
            m_myNewImage[posRotate] = m_myImage[posImage];
            m_myNewImage[posRotate + 1] = m_myImage[posImage + 1];
            m_myNewImage[posRotate + 2] = m_myImage[posImage + 2];
        }
    }

    std::swap(m_imageHeight, m_imageWidth);

	memcpy(m_myImage, m_myNewImage, m_imageWidth * m_imageHeight * 3) ;

    free(m_myNewImage);

    // update display
    SetSize(m_imageWidth, m_imageHeight) ;
	GetParent()->SetClientSize(GetSize()) ;
	Refresh(false) ;
}

void MyCanvas::RotateCounterClockwise()
{
    int i, j;
    int posImage, posRotate;
    // Original image is NxM
    int n, m;
    n = m_imageHeight;
    m = m_imageWidth;

    unsigned char* m_myNewImage = (unsigned char*)malloc(m_imageWidth * m_imageHeight * 3);

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            // rotatedImage[m - j - 1][i] = originalImage[i][j];
            posImage = i * m_imageWidth * 3 + j*3;
            posRotate = ((m - j - 1) * n * 3) + (i * 3);
            m_myNewImage[posRotate] = m_myImage[posImage];
            m_myNewImage[posRotate + 1] = m_myImage[posImage + 1];
            m_myNewImage[posRotate + 2] = m_myImage[posImage + 2];
        }
    }

    std::swap(m_imageHeight, m_imageWidth);

	memcpy(m_myImage, m_myNewImage, m_imageWidth * m_imageHeight * 3) ;

    free(m_myNewImage);

    // update display
    SetSize(m_imageWidth, m_imageHeight) ;
	GetParent()->SetClientSize(GetSize()) ;
	Refresh(false) ;
}

void MyCanvas::ZoomIn()
{
    int zoomHeight, zoomWidth, n, m;
    n = m_imageHeight;
    m = m_imageWidth;
    zoomHeight = (n - 1) * 2;
    zoomWidth = (m - 1) * 2;
    unsigned char* m_myNewImage = (unsigned char*)malloc(zoomWidth * zoomHeight * 3);

    int i, j;
    int posImage, posZoom;
    // Used to calculte the other indexes of the zoom image
    int posZoomAux1, posZoomAux2;

    for(i = 0; i < n; i++)
    {
        for(j = 0; j < m; j++)
        {
            // m_myNewImage[i*2][j*2] = m_myNewImage[i][j]
            posZoom = (i * 2  * zoomWidth * 3) + (j * 2 * 3);
            posImage = (i * m * 3) + j*3;
           // Check if posZoom is within the bounds of m_myNewImage
            if (posZoom + 2 < zoomWidth * zoomHeight * 3)
            {
                posImage = (i * m + j) * 3;

                // Copy values only if posImage is within bounds of m_myImage
                if (posImage + 2 < m * n * 3)
                {
                    m_myNewImage[posZoom] = m_myImage[posImage];
                    m_myNewImage[posZoom + 1] = m_myImage[posImage + 1];
                    m_myNewImage[posZoom + 2] = m_myImage[posImage + 2];
                }
            }
        }
    }

    // Interpolating the pixels of the columns
    for(i = 0; i < zoomHeight; i+=2)
    {
        for(j = 1; j < zoomWidth - 1; j+=2)
        {
            //m_myNewImage[i][j] = 0.5* (m_mynewImage[i][j-1] + my_newImage[i][j+1])
            posZoom = (i * zoomWidth * 3) + j*3;
            posZoomAux1 = (i * zoomWidth * 3) + ((j - 1) * 3);
            posZoomAux2 = (i * zoomWidth * 3) + ((j + 1) * 3);
            if (posZoomAux1 > 0 && posZoomAux2 < zoomWidth * zoomHeight * 3)
            {
                m_myNewImage[posZoom] = 0.5 * (m_myNewImage[posZoomAux1] + m_myNewImage[posZoomAux2]);
                m_myNewImage[posZoom + 1] = 0.5 * (m_myNewImage[posZoomAux1 + 1] + m_myNewImage[posZoomAux2 + 1]);
                m_myNewImage[posZoom + 2] = 0.5 * (m_myNewImage[posZoomAux1 + 2] + m_myNewImage[posZoomAux2 + 2]);
            }
        }
    }

    // Interpolating the pixels of the rows
    for(i = 1; i < zoomHeight - 1; i+=2)
    {
        for(j = 0; j < zoomWidth; j++)
        {
            // m_myNewImage[i][j] = 0.5 * (m_myNewImage[i - 1][j] + m_myNewImage[i + 1][j])
            posZoom = (i * zoomWidth * 3) + j*3;
            posZoomAux1 = ((i - 1) * zoomWidth * 3) + j*3;
            posZoomAux2 = ((i + 1) * zoomWidth * 3) + j*3;
            if (posZoomAux1 > 0 && posZoomAux2 < zoomWidth * zoomHeight * 3)
            {
            m_myNewImage[posZoom] = 0.5 * (m_myNewImage[posZoomAux1] + m_myNewImage[posZoomAux2]);
            m_myNewImage[posZoom + 1] = 0.5 * (m_myNewImage[posZoomAux1 + 1] + m_myNewImage[posZoomAux2 + 1]);
            m_myNewImage[posZoom + 2] = 0.5 * (m_myNewImage[posZoomAux1 + 2] + m_myNewImage[posZoomAux2 + 2]);
            }
        }
    }

    // Reallocating space for m_myImage
    unsigned char* newMyImage = (unsigned char*)realloc(m_myImage, zoomWidth * zoomHeight * 3);

    // If the program can't reallocate the memory for new image, just displays it on another window
    if(newMyImage == NULL)
    {
        m_imageViewer = new ImageViewerFrame("Zoom", wxSize(zoomWidth, zoomHeight), m_myNewImage, zoomWidth, zoomHeight);
        m_imageViewer->Show(true);
        free(m_myNewImage);
	    Refresh(false) ;
        return;
    }
    // If it can successfully allocate more memory, displays the new image in the main window
    // Copies the data from the zoom image to the displayed image
    m_myImage = newMyImage;
    memcpy(m_myImage, m_myNewImage, zoomHeight * zoomWidth * 3);
    free(m_myNewImage);

    m_imageHeight = zoomHeight;
    m_imageWidth = zoomWidth;

    // Update the display
    SetSize(m_imageWidth, m_imageHeight) ;
    GetParent()->SetClientSize(GetSize()) ;
    
	Refresh(false) ;
}

void MyCanvas::ZoomOut()
{
    // Reduction factors
    int sx, sy;
    wxString valueString = m_spinCtrl->GetValue();
    long longSx;
    if (valueString.ToLong(&longSx)) {
        sx = static_cast<int>(longSx);
        sx = std::max(1, std::min(255, sx));
    }
    wxString valueString2 = m_spinCtrl2->GetValue();
    long longSy;
    if (valueString2.ToLong(&longSy)) {
        sy = static_cast<int>(longSy);
        sy = std::max(1, std::min(255, sy));
    }

    int zoomHeight, zoomWidth;
    int i, j, iX, jY, n, m, divisor;
    int sumRed, sumGreen, sumBlue;
    int posImage, posZoom;
    int posX, posY;

    // Dimensions of the new image
    zoomHeight = static_cast<int>(m_imageHeight / sx) + (m_imageHeight % sx);
    zoomWidth = static_cast<int>(m_imageWidth / sx) + (m_imageWidth % sx);

    // Original image is a matrix N x M
    n = m_imageHeight;
    m = m_imageWidth;

    // Allocating space for the new image
    unsigned char* m_myNewImage = (unsigned char*)malloc(zoomHeight * zoomWidth * 3);

    for(i = 0; i < n; i += sx )
    {
        for(j = 0; j < m; j += sy)
        {
            sumRed = 0;
            sumGreen = 0;
            sumBlue = 0;
            divisor = 0;
            for(iX = 0; iX < sx; iX++)
            {
                for(jY = 0; jY < sy; jY++)
                {
                    // m_myNewImage[iX/sx][jY/jy] = sum of m_myImage in all the positions of sx and sy  
                    posImage = ((i + iX) * m_imageWidth * 3) + (j + jY)*3;
                    // Only sums pixels that are in the bounds of the image
                    if(!(posImage >= ((i + 1 + iX) * m_imageWidth * 3)))
                    {
                        sumRed += m_myImage[posImage];
                        sumGreen += m_myImage[posImage + 1];
                        sumBlue += m_myImage[posImage + 2];
                        divisor++;
                    }
                }
            }
            posZoom = ((i/sx) * zoomWidth * 3) + (j/sy) * 3;
            m_myNewImage[posZoom] = sumRed / divisor;
            m_myNewImage[posZoom + 1] = sumGreen / divisor;
            m_myNewImage[posZoom + 2] = sumBlue / divisor;
        }
    }

    // Reallocating space for m_myImage
    unsigned char* newMyImage = (unsigned char*)realloc(m_myImage, zoomWidth * zoomHeight * 3);

    // If the program can't reallocate the memory for new image, just displays it on another window
    if(newMyImage == NULL)
    {
        m_imageViewer = new ImageViewerFrame("Zoom", wxSize(zoomWidth, zoomHeight), m_myNewImage, zoomWidth, zoomHeight);
        m_imageViewer->Show(true);
        free(m_myNewImage);
	    Refresh(false) ;
        return;
    }
    // If it can successfully allocate more memory, displays the new image in the main window
    // Copies the data from the zoom image to the displayed image
    m_myImage = newMyImage;
    memcpy(m_myImage, m_myNewImage, zoomHeight * zoomWidth * 3);
    free(m_myNewImage);

    m_imageHeight = zoomHeight;
    m_imageWidth = zoomWidth;

    // Update the display
    SetSize(m_imageWidth, m_imageHeight) ;
    GetParent()->SetClientSize(GetSize()) ;
    
	Refresh(false) ;
}

void MyCanvas::PlotHistogram()
{
    int i;
    int posHist, maxValue;

    // Clearing the histogram array
    for(i = 0; i < 256; i++)
    {
        histogram[i] = 0;
    }

    // Calculates the intensity of each value
    for(i = 0; i < m_imageWidth * m_imageHeight * 3; i+=3)
    {
        posHist = m_myImage[i];
        histogram[posHist]++;
    }

    // Finds the value with the highest intensity
    maxValue = 0;
    for(i = 0; i < 256; i++)
    {
        if(histogram[i] > maxValue)
        {
            maxValue = histogram[i];
        }
    }

    // Normalizing the histogram
    for(i = 0; i < 256; i++)
    {
        histogram[i] = static_cast<int>(static_cast<double>(histogram[i]) / maxValue * 255);
    }
    // Creating a solid white image
    wxImage solidImage(768, 400);
    solidImage.SetRGB(wxRect(0, 0, 768, 400), 255, 255, 255);
    // Converting it to an array of bytes
    unsigned char* imageData = solidImage.GetData();
    // Changing the size of the canvas
    unsigned char* newMyImage = (unsigned char*)realloc(m_myImage, 768 * 400 * 3);
    if(newMyImage != NULL)
    {
        m_myImage = newMyImage;
        m_imageHeight = 400;
        m_imageWidth = 768;
        memcpy(m_myImage, imageData, 768 * 400 * 3);

        // Update the display
        SetSize(m_imageWidth, m_imageHeight) ;
        GetParent()->SetClientSize(GetSize()) ;
    }
    
    plotHistogram = 1;
    Refresh(false);
}

void MyCanvas::EqualizeHistogram()
{
    // Alpha: 255.0 / (number of pixels in the image)
    float alpha = 255.0 / (m_imageWidth * m_imageHeight);

    // Clearing the histogram array
    int hist[256] = {0};
    
    // Computing hist
    for (int i = 0; i < m_imageWidth * m_imageHeight * 3; i += 3)
    {
        int posHist = m_myImage[i];
        hist[posHist]++;
    }

    // Computing the cumulative hist
    int hist_cum[256] = {0};
    hist_cum[0] = alpha * hist[0];
    for (int i = 1; i < 256; i++)
    {
        hist_cum[i] = hist_cum[i - 1] + alpha * hist[i];
    }

    // Assigning new values to the image
    for (int i = 0; i < m_imageWidth * m_imageHeight * 3; i += 3)
    {
        int posHist = m_myImage[i];
        m_myImage[i] = hist_cum[posHist];
        m_myImage[i + 1] = hist_cum[posHist];
        m_myImage[i + 2] = hist_cum[posHist];
    }

    Refresh(false);
}

void MyCanvas::MatchHistogram()
{
    GrayScale(0);
    int hist_src[256] = {0};
    int hist_target[256] = {0};
    int hist_src_cum[256] = {0};
    int hist_target_cum[256] = {0};
    int HM[256] = {0};

    int posHist;

    // Computing hist
    for (int i = 0; i < m_imageWidth * m_imageHeight * 3; i += 3)
    {
        posHist = m_myImage[i];
        hist_src[posHist]++;
    }

    // Finds the value with the highest intensity
    int maxValue = 0;
    for(int i = 0; i < 256; i++)
    {
        if(hist_src[i] > maxValue)
        {
            maxValue = hist_src[i];
        }
    }

    // Normalizing the hist_src
    for(int i = 0; i < 256; i++)
    {
        hist_src[i] = static_cast<int>(static_cast<double>(hist_src[i]) / maxValue * 255);
    }

    // Compute cumulative histogram for hist_src
    hist_src_cum[0] = hist_src[0];
    for (int i = 1; i < 256; ++i) {
        hist_src_cum[i] = hist_src_cum[i - 1] + hist_src[i];
    }

    // Computing hist_target
    for (int i = 0; i < targetWidth * targetHeight * 3; i += 3)
    {
        posHist = m_myImageTarget[i];
        hist_target[posHist]++;
    }

    // Finds the value with the highest intensity
    maxValue = 0;
    for(int i = 0; i < 256; i++)
    {
        if(hist_target[i] > maxValue)
        {
            maxValue = hist_target[i];
        }
    }

    // Normalizing the hist_target
    for(int i = 0; i < 256; i++)
    {
        hist_target[i] = static_cast<int>(static_cast<double>(hist_target[i]) / maxValue * 255);
    }

    // Compute cumulative histogram for hist_target
    hist_target_cum[0] = hist_target[0];
    for (int i = 1; i < 256; ++i) {
        hist_target_cum[i] = hist_target_cum[i - 1] + hist_target[i];
    }

    // For each source shade, find the closest target shade with the most similar cumulative value
    for (int source_shade = 0; source_shade < 256; ++source_shade) {
        int min_difference = INT_MAX;
        int closest_target_shade = 0;

        for (int target_shade = 0; target_shade < 256; ++target_shade) {
            int difference = abs(hist_src_cum[source_shade] - hist_target_cum[target_shade]);
            if (difference < min_difference) {
                min_difference = difference;
                closest_target_shade = target_shade;
            }
        }

        HM[source_shade] = closest_target_shade;
    }

    // Assigning new values to the image
    for (int i = 0; i < m_imageWidth * m_imageHeight * 3; i += 3)
    {
        int posHist = m_myImage[i];
        m_myImage[i] = HM[posHist];
        m_myImage[i + 1] = HM[posHist];
        m_myImage[i + 2] = HM[posHist];
    }

    Refresh(false);
}

void MyCanvas::EqualizeHistogramColor()
{
    // Alpha: 255.0 / (number of pixels in the image)
    float alpha = 255.0 / (m_imageWidth * m_imageHeight);

    // Clearing the histogram array for each channel
    int hist[3][256] = {{0}, {0}, {0}};

    // Computing hist for each channel
    for (int i = 0; i < m_imageWidth * m_imageHeight; ++i)
    {
        for (int c = 0; c < 3; ++c)
        {
            int posHist = m_myImage[3 * i + c];
            hist[c][posHist]++;
        }
    }

    // Computing the cumulative hist for each channel
    int hist_cum[3][256] = {{0}, {0}, {0}};
    for (int c = 0; c < 3; ++c)
    {
        hist_cum[c][0] = alpha * hist[c][0];
        for (int i = 1; i < 256; ++i)
        {
            hist_cum[c][i] = hist_cum[c][i - 1] + alpha * hist[c][i];
        }
    }

    // Assigning new values to the image for each channel
    for (int x = 0; x < m_imageWidth; ++x)
    {
        for (int y = 0; y < m_imageHeight; ++y)
        {
            for (int c = 0; c < 3; ++c)
            {
                int posHist = m_myImage[3 * (y * m_imageWidth + x) + c];

                // Equalize each channel based on the renormalized cumulative histogram of its own values
                m_myImage[3 * (y * m_imageWidth + x) + c] = hist_cum[c][posHist];
            }
        }
    }

    Refresh(false);
}