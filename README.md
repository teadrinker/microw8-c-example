This is a C example for [MicroW8](https://exoticorn.github.io/microw8/), including build scripts, util funcs.

I do not really know much in depth about Clang or web assembly, so consider this random hacks and notes from the development of our demo [RE:FORM](https://demozoo.org/productions/371274/).

The hardest constraint for us seemed to be related to wasm module size, we started getting crashes somewhere around 110-120 kb, which is less than half of the stated capacity.
All the C static data and global variables are allocated starting at 0x14000 ("user memory"). Directly after that is the stack, (and the stack is reversed, starting from it's 
highest memory address, and allocating towards 0)

There is more usable memory above the stack, however the location is not known at compile time.

However, it seems we can just do this at runtime:

    void start() {
        unsigned char last_byte_before_extended_mem;
        static_alloc_init((&last_byte_before_extended_mem) + 1);
    }

Initially I did not figure this out, so I spent quite some time on a compressed-in-memory format for 8-bit audio and images.
However, since the wasm module 110-120 kb limit is the main concern, it's better to go for a better compression.

Also, remember that the audio thread has it's own separate memory, so you can, for instance, unpack audio samples to the screen.
This also enables running micromod separately on the main thread for sync, even though it uses a lot of global variables.
