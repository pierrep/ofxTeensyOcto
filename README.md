ofxTeensyOcto
=============

openFrameworks class + example to connect to Teensy 3.0 &amp; Octo library.
PLEASE NOTE: This project is currently setup to use 4 Teensy boards with a resolution of 64x64.  Each Teensy has technically 8 strips each with the strips at 128 pixels in length.  We cut the each strip at the 64th pixel and wire the second half so that it is positioned underneath.  The software will automatically flip the second strip so the graphics are displayed correctly...  A bit confusing to explain, might need to share photos soon.  In the meantime, ping me if you have questions.

##HOW TO USE
1. (i) key = Images
2. (v) key = Videos, press during to pause/play.
3. (d) key = demo mode, press during to toggle various modes.
4. (- or +) key = steps through videos, images.
5. (up or down) keys = control the LED brightness. (does not affect screen brightness)
6. (left or right) keys = control animation speeds (currently only affects Wave animation).
7. (r) key = rotates the display by 90 degrees.
8. (f) key = flips the display.
9. (t) key = is the panel test pattern.
