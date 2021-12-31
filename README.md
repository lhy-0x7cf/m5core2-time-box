
# M5Core2 LVGL Demo ESP-IDF

## Description
This project is a demo to use [LVGL](https://github.com/lvgl/lvgl) with [esp-idf](https://github.com/espressif/esp-idf) to write an app with GUI on [M5Stack Core2](https://docs.m5stack.com/en/core/core2) device.

## Build and Load
1. After cloning the repo, init and update the submodules
    ```bash
    git submodule init
    git submodule update
    ```
2. Launch esp-idf terminal and run the following commands
    ```bash
    idf.py build flash
    ```

## Note
Directly copy the `sdkconfig` file from https://github.com/imliubo/lvgl_port_M5Core2. Without this config file, I am not able to build the project correctly to make it run without issue. It will be nice to note down how to config the project with `idf.py menuconfig`.

For [LVGL](https://github.com/lvgl/lvgl), DO NOT USE any release branch and just use the master branch. Never make it work even with the config file mentioned above.
