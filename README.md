# ESP32-based camera with image streaming over an OLED display.

Playing around with my TTGO-Camera board. First intention was just to show the image stream from the camera directly over the OLED display. It gradually was enriched with varios image depths. See demo with 3-pixel depth.

![oled-cam demo](demo/oled_cam_demo.gif)

Different image depths are achived using more pixels for intensity representation. For instance, 1-pixel depth would look like binary (2 levels of gradation) image with a threshold set on 256/2. Taking 2 pixels will render 3 levels of gradation: black (px < 256/3), grey (255/3 < px < 255*2/3), and white (x > 255*3/2). More pixels -> more intensity levels -> more depth.

## License

The source code for the project is licensed under the MIT license, which you can find in the LICENSE.txt file.
