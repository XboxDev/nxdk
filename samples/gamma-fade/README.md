gamma-fade
==========

Example of using GPU gamma ramps for fade effects.

The benefit of this technique is that it doesn't require any memory access.
So if your scene already has a lot of overdraw, this might be a suitable solution.
It only costs a bit of CPU time (while setting gamma ramps for GPU).

However, this independence from memory can also be a drawback.
As it is applied during video-output, it is a global effect.
So it will also always affect your HUD or other elements which might not be supposed to be part of the fade.

Also, because it is controlled by the CPU, it requires some form of synchronization.
You should try to do it during vertical-blanking to avoid tearing.

This sample shows 2 common styles of fade effects:

* Mix:
    This linearly interpolates between a constant color and the intended color.

* Cinematic:
    Similar to how a fade looks when done in film.
    This is closer to an add (fade-to-white) or subtract (fade-to-black).

In practice, you could extend this effect by fine-tuning it per component.
You can also set custom curves to suit your application.
In a real application you might also want to convolve the fade with the actual gamma curve.

Each style is shown with 2 interpolations:

* Linear:
    This animates the fade with constant "speed".

* Smooth:
    This controls the animation speed so it performs ease-in and ease-out.

In a real application you'd likely use a custom interpolator (a custom curve).
You'd also vary the duration of the animation.
