# esp-weather-station

Displays current weather data from open weather (https://openweathermap.org/) and your own sensor data from IotFreezer (https://iotfreezer.com/).

Adapted for 2.4in screen (I used ILI9341, for all supported options, see TFT_eSPI's docs).

Uses the `ArduinoJSON` library for json parsing, `TFT_eSPI` for drawing on the screen.

Heavily inspired by https://github.com/Bodmer/OpenWeather. Built with platformIO.

![Image](https://github.com/xtrinch/esp-weather-station/blob/master/images/image.jpg)

## Usage

Make sure to clone with `git clone --recursive REPO-URL` to make sure `ESP-make-easy` submodule is cloned along.

Before flashing, copy `import_env.example.py` to `import_env.py` and fill in your own configuration.

Make sure you upload the `/data` folder where the images and fonts reside to the ESP before flashing. You can do that with `pio run --target uploadfs` or just use the platformio addon for vscode and click on `Upload Filesystem Image`.

To generate a BMP that works with this lib, use image magick:
`magick convert *.bmp -background black -alpha remove -compress none -depth 24 %d.bmp`

## Steps that follow

I also did some manual soldering work and 3d printed a case for it (slides into a wall mount, so you can easily flash and upgrade it):

![Image](https://github.com/xtrinch/esp-weather-station/blob/master/images/weatherstation.jpg)

The repository can be found at https://github.com/xtrinch/parametric-display-casing. It's parametric, so you just measure the actual dimensions of your finished soldered product and enter them into the FreeCAD spreadsheet and print it.

