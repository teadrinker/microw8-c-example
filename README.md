### About the example

This is a C example for [MicroW8](https://exoticorn.github.io/microw8/), including build scripts, util funcs.
Music player and visualization using [Micromod](https://github.com/martincameron/micromod) by Martin Cameron.

I do not really know much in depth about Clang or WebAssembly, 
so consider this random hacks and notes from the development of 
our demo [RE:FORM](https://demozoo.org/productions/371274/).
Note that there is also a [longer write-up about making the demo](https://teadrinker.net/blog/re-form.html).

### Setup

You'll need:
 * [uw8.exe](https://github.com/exoticorn/microw8/releases) - place it in the same dir as this readme file
 * [clang.exe](https://clang.llvm.org/)
 * [wasm-opt.exe](https://github.com/WebAssembly/binaryen/releases) 

(we used clang 16.0.0, and wasm-opt version 114)

The build bat files are mostly just windows variations of the [MicroW8 C example](https://github.com/exoticorn/microw8/tree/master/examples/c)

[build and run fast.bat](https://github.com/teadrinker/microw8-c-example/blob/main/example-project/build%20and%20run%20fast.bat) is the one you would use for development, and build.bat when you want to ship and make sure it's as small as possible.

### Working around weird constraints

The hardest constraint for us seemed to be related to wasm module size, 
we started getting crashes somewhere around 110-120 kb, which is less than 
half of the stated capacity. All the C static data and global variables 
are allocated starting at 0x14000 ("user memory"). Directly after that 
is the stack, (and the stack is reversed, starting from it's highest 
memory address, and allocating towards 0)

There is more usable memory above the stack, however the starting 
address of this memory is not known at compile time.

However, it seems we can just do this at runtime:

    void start() {
        unsigned char last_byte_before_extended_mem;
        static_alloc_init((&last_byte_before_extended_mem) + 1);
    }

Initially I did not figure this out, so I spent quite some 
time on a compressed-in-memory format for 8-bit audio and images.
However, since the wasm module 110-120 kb limit is the main concern, 
it's probably better to go for a better quality compression, and then
unpack to the remaining memory...


### Sound is separate

The snd() audio callback has it's own thread, and also it's own separate memory. 
So you can, for instance, unpack audio samples to the screen.
This also enables running Micromod separately on the main thread 
for sync, even though it uses a lot of global variables.
(See cart.c for an example of how to do this)

It's a bit of a convoluted solution to run two instances of Micromod, however
as far as I know, there is currently no way of sending data from the audio thread to the main thread.

To get some more samples into RE-FORM, I used lz compression for 
patterns and samples (and some samples use my compressed-in-memory-format).
But ideally for audio, you would use a lossy format like [QOA](https://github.com/phoboslab/qoa) or [pulsejet](https://yupferris.github.io/blog/2021/06/07/pulsejet-sample-compression-codec-for-64k.html). Next time...

Would love to see more demos on this platform!
