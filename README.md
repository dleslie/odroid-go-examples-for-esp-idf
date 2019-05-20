# odroid-go-examples-for-esp-idf

This is a port of many of the ODROID-GO Arduino examples to the ESP-IDF toolchain.

Provided also is an install script to bootstrap the necessary ESP-IDF toolchain variants on Ubuntu-based Linux environments.

# Making fw

Create the tile:

```ffmpeg -i 86x48px-Arduino_Logo.png -f rawvideo -pix_fmt rgb565 tile.raw```

Run mkfw:

```./mkfw test tile.raw 0 16 1048576 app build/odroid-go-examples.bin```