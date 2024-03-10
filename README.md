# CM3070 Final Project - Yard Sentry - Jordan Hiebert

This repo is the code for Jordan Hiebert's CM3070 Final Project: Yard Sentry. It is functional prototype with the following features:

* 250 kilobyte Tensorflow Lite neural network to recognize people in images captured by a camera
* Web server to monitor and interact with the project
* Home Assistant integration to undertake automations and alerting based on person detections

## Requirements

### Hardware
This project has been built and tested on the ESP32-CAM module by AI-Thinker, but in theory it should be possible to adapt to others with relatively minor tweaks.

### ESP IDF

This project was created using Espressif's IDF, which can be installed via IDE plugins and also manually if desired.

#### IDE
The easiest (and recommended by Espressif) way to install the ESP IDF is via IDE extensions. Both Eclipse and VS Code are supported.

* [VS Code](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/install.md)
* [Eclipse](https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md)

#### Manual installation

Follow the instructions of the
[ESP-IDF get started guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html)
to setup the toolchain and the ESP-IDF itself.

The next steps assume that the
[IDF environment variables are set](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/index.html#step-4-set-up-the-environment-variables) :

 * The `IDF_PATH` environment variable is set
 * `idf.py` and Xtensa-esp32 tools (e.g. `xtensa-esp32-elf-gcc`) are in `$PATH`

## Build and run

### Getting started

1. Clone the project
2. Navigate to project root directory

### Set target and build

Set the chip target:

```
idf.py set-target esp32
```

Then build with `idf.py`
```
idf.py build
```

### Flashing

To flash and monitor (replace `/dev/ttyUSB0` with the device serial port):
```
idf.py --port /dev/ttyUSB0 flash monitor
```

Use `Ctrl+]` to exit.

### Accessing the device web interface

You can access the device web interface with any web browser, using the IP address output on the debug monitor: http://<IP_ADDRESS>

### MQTT integration

This project publishes detection events based on configuratiokn in mqtt.h.
