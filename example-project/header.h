#ifndef DEMODEV

#define IMPORT(MODULE, NAME) __attribute__((import_module(MODULE), import_name(NAME)))

IMPORT("env", "sin" )                extern float sin    (float);
IMPORT("env", "cos" )                extern float cos    (float);
IMPORT("env", "tan" )                extern float tan    (float);
IMPORT("env", "asin")                extern float asin   (float);
IMPORT("env", "acos")                extern float acos   (float);
IMPORT("env", "exp" )                extern float exp    (float);
IMPORT("env", "log" )                extern float log    (float);
IMPORT("env", "atan")                extern float atan   (float);
IMPORT("env", "atan2")               extern float atan2  (float, float);
IMPORT("env", "pow" )                extern float pow    (float, float);
IMPORT("env", "fmod")                extern float fmod   (float, float); // note, Microw8 uses this fmod = x - floor(x / y) * y
IMPORT("env", "abs")                 extern float abs   (float);

IMPORT("env", "random")              extern int random      ();
IMPORT("env", "randomf")             extern float randomf   ();
IMPORT("env", "randomSeed")          extern void randomSeed (int);

IMPORT("env", "cls")            extern void cls(int); 
IMPORT("env", "rectangle")           extern void rectangle(float, float, float, float, int); 
IMPORT("env", "circle")              extern void circle(float, float, float, int); 
IMPORT("env", "circleOutline")       extern void circleOutline(float, float, float, int); 
IMPORT("env", "line")                extern void line(float, float, float, float, int); 
IMPORT("env", "hline")               extern void hline(int, int, int, int); 
IMPORT("env", "setPixel")            extern void setPixel(int, int, int); 
IMPORT("env", "getPixel")            extern int getPixel(int, int); 
IMPORT("env", "isButtonPressed")     extern int isButtonPressed(int); 

IMPORT("env", "time")                extern float time();
IMPORT("env", "playNote")            extern void playNote(int, int); // Triggers a note (1-127) on the given channel (0-3).
IMPORT("env", "printString")         extern void printString(char *txt);
IMPORT("env", "printChar")           extern void printChar(int); 
IMPORT("env", "printInt")            extern void printInt(int); 
IMPORT("env", "setTextColor")        extern void setTextColor(int); 
IMPORT("env", "setBackgroundColor")  extern void setBackgroundColor(int); 
IMPORT("env", "setCursorPosition")   extern void setCursorPosition(int, int); 

#define OPT_INL __attribute__((always_inline))

OPT_INL float fabs(float x) { return __builtin_fabsf(x); }
OPT_INL float sqrt(float x) { return __builtin_sqrtf(x); }
OPT_INL float floor(float x) { return __builtin_floorf(x); }
OPT_INL float min(float a, float b) { return __builtin_fminf(a, b); }
OPT_INL float max(float a, float b) { return __builtin_fmaxf(a, b); }
OPT_INL float sign(float a) { return __builtin_copysignf(1.0f,a);}
OPT_INL float fract(float x) { return x-floor(x); }
OPT_INL int   iabs(int x) { return __builtin_abs(x); }  // https://github.com/llvm-mirror/clang/blob/master/include/clang/Basic/Builtins.def

OPT_INL float saturate(float a) { return max(0.0f,min(a, 1.0f)); }
OPT_INL float clamp(float a, float b,float c) { return max(b,min(a, c)); }

OPT_INL void* memset(void* ptr, int value, unsigned int num)
{
    unsigned char* p = (unsigned char*)ptr;
    unsigned char v = value;
    for (int i = 0; i < num; i++) p[i] = v;
    return p;
}

// MicroW8 Memory Map
#define FRAMENR ((unsigned char*)0x4C)
#define FRAMEBUFFER_SOUND_THREAD ((unsigned char*)0x78) // same address, but different mem for microW8 (but dev framework need other address)
#define FRAMEBUFFER ((unsigned char*)0x78)
#define PALETTE     ((unsigned char*)0x13000)
#define GAMEPAD     ((unsigned char*)0x44)
#define TIME_MS     ((unsigned char*)0x40)
#define FONT        ((unsigned char*)0x13400)
//#define USER_MEM    ((unsigned char*)0x14000) // note, C globals are at USER_MEM
#define SNDGES0     ((unsigned char*)0x50)
#define SNDGES1     ((unsigned char*)0x56)
#define SNDGES2     ((unsigned char*)0x5c)
#define SNDGES3     ((unsigned char*)0x62)

unsigned char* static_alloc_ptr; // points to free memory
void static_alloc_init(unsigned char* ptr_to_stack_var) { static_alloc_ptr = ptr_to_stack_var; }


#endif

int static_alloc_needs_init() { return static_alloc_ptr == 0; }
unsigned char* static_alloc(int byte_count) {
    unsigned char* buffer = static_alloc_ptr;
    static_alloc_ptr += byte_count;
    return buffer;
}


#define BUTTON_UP    0x0
#define BUTTON_DOWN  0x1
#define BUTTON_LEFT  0x2
#define BUTTON_RIGHT 0x3
#define BUTTON_A     0x4
#define BUTTON_B     0x5
#define BUTTON_X     0x6
#define BUTTON_Y     0x7


#define SNDGES_CHANNEL_STRIDE 0x6
#define SNDGES_CTRL 0
#define SNDGES_PULS 1
#define SNDGES_FINE 2
#define SNDGES_NOTE 3
#define SNDGES_ENVA 4
#define SNDGES_ENVB 5
#define SNDGES_CTRL_NOTEON 1
#define SNDGES_CTRL_TRIGGER 2
#define SNDGES_CTRL_FILTER (4|8)
#define SNDGES_CTRL_FILTER_OFF 0
#define SNDGES_CTRL_FILTER_6DB 4
#define SNDGES_CTRL_FILTER_PF0 8
#define SNDGES_CTRL_FILTER_PF1 (4|8)
#define SNDGES_CTRL_WIDE 16
#define SNDGES_CTRL_RING 32
#define SNDGES_CTRL_WAVE (64|128)
#define SNDGES_CTRL_WAVE_RECT 0
#define SNDGES_CTRL_WAVE_SAW 64
#define SNDGES_CTRL_WAVE_TRIANGLE 64
#define SNDGES_CTRL_WAVE_NOISE (64|128)

#define pi 3.14159265358979311f
#define WIDTH 320
#define HEIGHT 240

#define PIXELTYPE unsigned char
#define MICROW8_HEADER 
