gamma
=====

Example of using GPU gamma ramps for tonemapping and effects.

By not setting a linear ramp, you are typically reducing the amount of colors that you can output to the display.
It might still be beneficial in some situations (correcting for bad display).

In practice, tonemapping is not useful on original Xbox.
The GPU does not support higher framebuffer precision than 8-bit integers per channel.
So when working in linear space, it will quickly lead to quantization artifacts.
