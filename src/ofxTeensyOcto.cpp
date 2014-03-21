//
//      OF Teensy/OCTO class by Jason Walters @ BBDO ...
//      Original Processing/JS code by Paul Stoffregen/PJRC.COM
//
//      Last revision by Jason Walters on March 21st, 2014
//      Made with openFrameworks 0.80
//
//--------------------------------------------------------------

#include "ofxTeensyOcto.h"

//--------------------------------------------------------------
void ofxTeensyOcto::setup(int _ledWidth, int _ledHeight, int _stripsPerPort, int _numPorts)
{
    // LED variables
    ledWidth = _ledWidth;                       // LED max width
    ledHeight = _ledHeight;                     // LED max height
    stripsPerPort = _stripsPerPort;             // LED strips per teensy
    numPortsMain = _numPorts;                   // number of teensy ports
    numPorts = 0;                               // teensy ports counter
    maxPorts = 8;                               // max teensy ports
    
    dataSize = ((ledWidth * (ledHeight * stripsPerPort)) * 3) + 3;
    
    // LED arrays
    ledSerial = new ofSerial[maxPorts];
    ledArea = new ofRectangle[maxPorts];
    ledLayout = new bool[maxPorts];
    ledImage = new ofImage[maxPorts];
    colors = new ofColor[ledWidth * (ledHeight * stripsPerPort * numPortsMain)];
    pixels1.allocate(ledWidth, ledHeight*stripsPerPort*numPortsMain, OF_PIXELS_RGB);
    pixels2.allocate(ledWidth, ledHeight*stripsPerPort*numPortsMain, OF_PIXELS_RGB);
    ledData = new unsigned char[((ledWidth * (ledHeight * stripsPerPort)) * 3) + 3];
    
    // let's list our serial devices
    ledSerial[numPorts].listDevices();
    vector <ofSerialDeviceInfo> deviceList = ledSerial[numPorts].getDeviceList();
    ofSleepMillis(20);    
}

//--------------------------------------------------------------
void ofxTeensyOcto::serialConfigure(string portName, float _xoffset, float _yoffset, float _widthPct, float _heightPct, int _direction)
{
    int baud = 115200;
    ledSerial[numPorts].setup(portName, baud);
    ledSerial[numPorts].writeByte('?');         // send an initial character
    ofSleepMillis(50);
    
    // only store the info and increase numPorts if Teensy responds properly
    ledImage[numPorts].allocate(ledWidth, ledHeight * stripsPerPort, OF_IMAGE_COLOR);
    ledArea[numPorts].set(_xoffset, _yoffset, _widthPct, _heightPct);
    ledLayout[numPorts] = _direction == 0; // affects layout > pixel direction
    numPorts++;
}

// image2data converts an image to OctoWS2811's raw data format.
// The number of vertical pixels in the image must be a multiple
// of 8.  The data array must be the proper size for the image.
//--------------------------------------------------------------
void ofxTeensyOcto::image2data(ofImage image, unsigned char* data, bool layout)
{    
    int offset = 3;
    int x, y, xbegin, xend, xinc, mask;
    int linesPerPin = image.height / 8;
    int* pixel = new int[8];
    
    // get the copied image pixels
    pixels2 = image.getPixelsRef();

    // 2d array of our pixel colors
    for (int x = 0; x < ledWidth; x++)
    {
        for (int y = 0; y < (ledHeight * stripsPerPort * numPortsMain); y++)
        {
            int loc = x + y * ledWidth;
            colors[loc] = pixels2.getColor(x, y);
        }
    }
    
    for (y = 0; y < linesPerPin; y++)
    {
        if ((y & 1) == (layout ? 0 : 1))
        {
            // even numbered rows are left to right
            xbegin = 0;
            xend = image.width;
            xinc = 1;
        }
        else
        {
            // odd numbered rows are right to left
            xbegin = image.width - 1;
            xend = -1;
            xinc = -1;
        }
        
        for (x = xbegin; x != xend; x += xinc)
        {
            for (int i=0; i < 8; i++)
            {
                int temploc = x + (y + linesPerPin * i) * image.width;
                pixel[i] = colors[temploc].getHex();
                pixel[i] = colorWiring(pixel[i]);
            }
            
            // convert 8 pixels to 24 bytes
            for (mask = 0x800000; mask != 0; mask >>= 1)
            {
                unsigned char b = 0;
                for (int i=0; i < 8; i++)
                {
                    if ((pixel[i] & mask) != 0) b |= (1 << i);
                }
                data[offset++] = b;
            }
        }
    }
}

//--------------------------------------------------------------
void ofxTeensyOcto::update()
{
    serialWrite();  // send our data via serial
}

//--------------------------------------------------------------
void ofxTeensyOcto::serialWrite()
{
    for (int i=0; i < numPorts; i++)
    {
        // copy a portion of the movie's image to the LED image
        float xoffset = percentage(ledWidth, ledArea[i].x);
        float yoffset = percentage((ledHeight * stripsPerPort * numPortsMain), ledArea[i].y);
        float xwidth =  percentage(ledWidth, ledArea[i].getWidth());
        float yheight = percentage((ledHeight * stripsPerPort * numPortsMain), ledArea[i].getHeight());
        
        // grabs the screen so we can convert to pixels
        ledImage[i].setFromPixels(pixels1);
        ledImage[i].crop(xoffset, yoffset, xwidth, yheight);
        
        image2data(ledImage[i], ledData, ledLayout[i]);
        
        ledData[0] = '*';  // first Teensy is the frame sync master
        
        int nBytesWritten = 0;
        while (nBytesWritten < dataSize){
            nBytesWritten += ledSerial[i].writeBytes( &ledData[nBytesWritten], dataSize );
        }
    }
}
