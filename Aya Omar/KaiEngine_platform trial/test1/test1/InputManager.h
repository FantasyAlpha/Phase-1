/*=======================================================================================================================
NOTE(kai): This file can:
1) process events (using either SDL or GLFW)
2) get key state (pressed or released
=======================================================================================================================*/

#pragma once

#include "WindowManager.h"

//Key codes according to GLFW
#if MODE_TYPE == GLFW_MODE
typedef enum {
/* The unknown key */
	KEY_UNKNOWN = -1,

/* Printable keys */
	KEY_SPACE = 32,
	KEY_APOSTROPHE = 39,  /* ' */
	KEY_COMMA = 44, /* , */
	KEY_MINUS = 45,  /* - */
	KEY_PERIOD = 46,  /* . */
	KEY_SLASH = 47,  /* / */
	KEY_0 = 48,
	KEY_1 = 49,
	KEY_2 = 50,
	KEY_3 = 51,
	KEY_4 = 52,
	KEY_5 = 53,
	KEY_6 = 54,
	KEY_7 = 55,
	KEY_8 = 56,
	KEY_9 = 57,
	KEY_SEMICOLON = 59,  /* ; */
	KEY_EQUAL = 61,  /* = */
	KEY_A = 65,
	KEY_B = 66,
	KEY_C = 67,
	KEY_D = 68,
	KEY_E = 69,
	KEY_F = 70,
	KEY_G = 71,
	KEY_H = 72,
	KEY_I = 73,
	KEY_J = 74,
	KEY_K = 75,
	KEY_L = 76,
	KEY_M = 77,
	KEY_N = 78,
	KEY_O = 79,
	KEY_P = 80,
	KEY_Q = 81,
	KEY_R = 82,
	KEY_S = 83,
	KEY_T = 84,
	KEY_U = 85,
	KEY_V = 86,
	KEY_W = 87,
	KEY_X = 88,
	KEY_Y = 89,
	KEY_Z = 90,
	KEY_LEFT_BRACKET = 91,  /* [ */
	KEY_BACKSLASH = 92,  /* \ */
	KEY_RIGHT_BRACKET = 93,  /* ] */
	KEY_GRAVE_ACCENT = 96,  /* ` */
	KEY_WORLD_1 = 161, /* non-US #1 */
	KEY_WORLD_2 = 162, /* non-US #2 */

/* Function keys */
	KEY_ESCAPE = 256,
	KEY_ENTER = 257,
	KEY_TAB = 258,
	KEY_BACKSPACE = 259,
	KEY_INSERT = 260,
	KEY_DELETE = 261,
	KEY_RIGHT = 262,
	KEY_LEFT = 263,
	KEY_DOWN = 264,
	KEY_UP = 265,
	KEY_PAGE_UP = 266,
	KEY_PAGE_DOWN = 267,
	KEY_HOME = 268,
	KEY_END = 269,
	KEY_CAPS_LOCK = 280,
	KEY_SCROLL_LOCK = 281,
	KEY_NUM_LOCK = 282,
	KEY_PRINT_SCREEN = 283,
	KEY_PAUSE = 284,
	KEY_F1 = 290,
	KEY_F2 = 291,
	KEY_F3 = 292,
	KEY_F4 = 293,
	KEY_F5 = 294,
	KEY_F6 = 295,
	KEY_F7 = 296,
	KEY_F8 = 297,
	KEY_F9 = 298,
	KEY_F10 = 299,
	KEY_F11 = 300,
	KEY_F12 = 301,
	KEY_F13 = 302,
	KEY_F14 = 303,
	KEY_F15 = 304,
	KEY_F16 = 305,
	KEY_F17 = 306,
	KEY_F18 = 307,
	KEY_F19 = 308,
	KEY_F20 = 309,
	KEY_F21 = 310,
	KEY_F22 = 311,
	KEY_F23 = 312,
	KEY_F24 = 313,
	KEY_F25 = 314,
	KEY_KP_0 = 320,
	KEY_KP_1 = 321,
	KEY_KP_2 = 322,
	KEY_KP_3 = 323,
	KEY_KP_4 = 324,
	KEY_KP_5 = 325,
	KEY_KP_6 = 326,
	KEY_KP_7 = 327,
	KEY_KP_8 = 328,
	KEY_KP_9 = 329,
	KEY_KP_DECIMAL = 330,
	KEY_KP_DIVIDE = 331,
	KEY_KP_MULTIPLY = 332,
	KEY_KP_SUBTRACT = 333,
	KEY_KP_ADD = 334,
	KEY_KP_ENTER = 335,
	KEY_KP_EQUAL = 336,
	KEY_LEFT_SHIFT = 340,
	KEY_LEFT_CONTROL = 341,
	KEY_LEFT_ALT = 342,
	KEY_LEFT_SUPER = 343,
	KEY_RIGHT_SHIFT = 344,
	KEY_RIGHT_CONTROL = 345,
	KEY_RIGHT_ALT = 346,
	KEY_RIGHT_SUPER = 347,
	KEY_MENU = 348,
	KEY_LAST = KEY_MENU
} KEY;

//Key codes according to SDL
#elif MODE_TYPE == SDL_MODE
	
typedef enum
{
	KEY_UNKNOWN = 0,

	/**
	*  \name Usage page 0x07
	*
	*  These values are from usage page 0x07 (USB keyboard page).
	*/
	/*@{*/

	KEY_A = 4,
	KEY_B = 5,
	KEY_C = 6,
	KEY_D = 7,
	KEY_E = 8,
	KEY_F = 9,
	KEY_G = 10,
	KEY_H = 11,
	KEY_I = 12,
	KEY_J = 13,
	KEY_K = 14,
	KEY_L = 15,
	KEY_M = 16,
	KEY_N = 17,
	KEY_O = 18,
	KEY_P = 19,
	KEY_Q = 20,
	KEY_R = 21,
	KEY_S = 22,
	KEY_T = 23,
	KEY_U = 24,
	KEY_V = 25,
	KEY_W = 26,
	KEY_X = 27,
	KEY_Y = 28,
	KEY_Z = 29,

	KEY_1 = 30,
	KEY_2 = 31,
	KEY_3 = 32,
	KEY_4 = 33,
	KEY_5 = 34,
	KEY_6 = 35,
	KEY_7 = 36,
	KEY_8 = 37,
	KEY_9 = 38,
	KEY_0 = 39,

	KEY_RETURN = 40,
	KEY_ESCAPE = 41,
	KEY_BACKSPACE = 42,
	KEY_TAB = 43,
	KEY_SPACE = 44,

	KEY_MINUS = 45,
	KEY_EQUALS = 46,
	KEY_LEFTBRACKET = 47,
	KEY_RIGHTBRACKET = 48,
	KEY_BACKSLASH = 49, /**< Located at the lower left of the return
						*   key on ISO keyboards and at the right end
						*   of the QWERTY row on ANSI keyboards.
						*   Produces REVERSE SOLIDUS (backslash) and
						*   VERTICAL LINE in a US layout, REVERSE
						*   SOLIDUS and VERTICAL LINE in a UK Mac
						*   layout, NUMBER SIGN and TILDE in a UK
						*   Windows layout, DOLLAR SIGN and POUND SIGN
						*   in a Swiss German layout, NUMBER SIGN and
						*   APOSTROPHE in a German layout, GRAVE
						*   ACCENT and POUND SIGN in a French Mac
						*   layout, and ASTERISK and MICRO SIGN in a
						*   French Windows layout.
						*/
	KEY_NONUSHASH = 50, /**< ISO USB keyboards actually use this code
						*   instead of 49 for the same key, but all
						*   OSes I've seen treat the two codes
						*   identically. So, as an implementor, unless
						*   your keyboard generates both of those
						*   codes and your OS treats them differently,
						*   you should generate KEY_BACKSLASH
						*   instead of this code. As a user, you
						*   should not rely on this code because SDL
						*   will never generate it with most (all?)
						*   keyboards.
						*/
	KEY_SEMICOLON = 51,
	KEY_APOSTROPHE = 52,
	KEY_GRAVE = 53, /**< Located in the top left corner (on both ANSI
				*   and ISO keyboards). Produces GRAVE ACCENT and
				*   TILDE in a US Windows layout and in US and UK
				*   Mac layouts on ANSI keyboards, GRAVE ACCENT
				*   and NOT SIGN in a UK Windows layout, SECTION
				*   SIGN and PLUS-MINUS SIGN in US and UK Mac
				*   layouts on ISO keyboards, SECTION SIGN and
				*   DEGREE SIGN in a Swiss German layout (Mac:
				*   only on ISO keyboards), CIRCUMFLEX ACCENT and
				*   DEGREE SIGN in a German layout (Mac: only on
				*   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
				*   French Windows layout, COMMERCIAL AT and
				*   NUMBER SIGN in a French Mac layout on ISO
				*   keyboards, and LESS-THAN SIGN and GREATER-THAN
				*   SIGN in a Swiss German, German, or French Mac
				*   layout on ANSI keyboards.
				*/
	KEY_COMMA = 54,
	KEY_PERIOD = 55,
	KEY_SLASH = 56,

	KEY_CAPSLOCK = 57,

	KEY_F1 = 58,
	KEY_F2 = 59,
	KEY_F3 = 60,
	KEY_F4 = 61,
	KEY_F5 = 62,
	KEY_F6 = 63,
	KEY_F7 = 64,
	KEY_F8 = 65,
	KEY_F9 = 66,
	KEY_F10 = 67,
	KEY_F11 = 68,
	KEY_F12 = 69,

	KEY_PRINTSCREEN = 70,
	KEY_SCROLLLOCK = 71,
	KEY_PAUSE = 72,
	KEY_INSERT = 73, /**< insert on PC, help on some Mac keyboards (but
					 does send code 73, not 117) */
	KEY_HOME = 74,
	KEY_PAGEUP = 75,
	KEY_DELETE = 76,
	KEY_END = 77,
	KEY_PAGEDOWN = 78,
	KEY_RIGHT = 79,
	KEY_LEFT = 80,
	KEY_DOWN = 81,
	KEY_UP = 82,

	KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards
														*/
	KEY_KP_DIVIDE = 84,
	KEY_KP_MULTIPLY = 85,
	KEY_KP_MINUS = 86,
	KEY_KP_PLUS = 87,
	KEY_KP_ENTER = 88,
	KEY_KP_1 = 89,
	KEY_KP_2 = 90,
	KEY_KP_3 = 91,
	KEY_KP_4 = 92,
	KEY_KP_5 = 93,
	KEY_KP_6 = 94,
	KEY_KP_7 = 95,
	KEY_KP_8 = 96,
	KEY_KP_9 = 97,
	KEY_KP_0 = 98,
	KEY_KP_PERIOD = 99,

	KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
								  *   keyboards have over ANSI ones,
								*   located between left shift and Y.
								*   Produces GRAVE ACCENT and TILDE in a
								*   US or UK Mac layout, REVERSE SOLIDUS
								*   (backslash) and VERTICAL LINE in a
								*   US or UK Windows layout, and
								*   LESS-THAN SIGN and GREATER-THAN SIGN
								*   in a Swiss German, German, or French
								*   layout. */
	KEY_APPLICATION = 101, /**< windows contextual menu, compose */
	KEY_POWER = 102, /**< The USB document says this is a status flag,
						*   not a physical key - but some Mac keyboards
						*   do have a power key. */
	KEY_KP_EQUALS = 103,
	KEY_F13 = 104,
	KEY_F14 = 105,
	KEY_F15 = 106,
	KEY_F16 = 107,
	KEY_F17 = 108,
	KEY_F18 = 109,
	KEY_F19 = 110,
	KEY_F20 = 111,
	KEY_F21 = 112,
	KEY_F22 = 113,
	KEY_F23 = 114,
	KEY_F24 = 115,
	KEY_EXECUTE = 116,
	KEY_HELP = 117,
	KEY_MENU = 118,
	KEY_SELECT = 119,
	KEY_STOP = 120,
	KEY_AGAIN = 121,   /**< redo */
	KEY_UNDO = 122,
	KEY_CUT = 123,
	KEY_COPY = 124,
	KEY_PASTE = 125,
	KEY_FIND = 126,
	KEY_MUTE = 127,
	KEY_VOLUMEUP = 128,
	KEY_VOLUMEDOWN = 129,
	/* not sure whether there's a reason to enable these */
	/*     KEY_LOCKINGCAPSLOCK = 130,  */
	/*     KEY_LOCKINGNUMLOCK = 131, */
	/*     KEY_LOCKINGSCROLLLOCK = 132, */
	KEY_KP_COMMA = 133,
	KEY_KP_EQUALSAS400 = 134,

	KEY_INTERNATIONAL1 = 135, /**< used on Asian keyboards, see
	footnotes in USB doc */
	KEY_INTERNATIONAL2 = 136,
	KEY_INTERNATIONAL3 = 137, /**< Yen */
	KEY_INTERNATIONAL4 = 138,
	KEY_INTERNATIONAL5 = 139,
	KEY_INTERNATIONAL6 = 140,
	KEY_INTERNATIONAL7 = 141,
	KEY_INTERNATIONAL8 = 142,
	KEY_INTERNATIONAL9 = 143,
	KEY_LANG1 = 144, /**< Hangul/English toggle */
	KEY_LANG2 = 145, /**< Hanja conversion */
	KEY_LANG3 = 146, /**< Katakana */
	KEY_LANG4 = 147, /**< Hiragana */
	KEY_LANG5 = 148, /**< Zenkaku/Hankaku */
	KEY_LANG6 = 149, /**< reserved */
	KEY_LANG7 = 150, /**< reserved */
	KEY_LANG8 = 151, /**< reserved */
	KEY_LANG9 = 152, /**< reserved */

	KEY_ALTERASE = 153, /**< Erase-Eaze */
	KEY_SYSREQ = 154,
	KEY_CANCEL = 155,
	KEY_CLEAR = 156,
	KEY_PRIOR = 157,
	KEY_RETURN2 = 158,
	KEY_SEPARATOR = 159,
	KEY_OUT = 160,
	KEY_OPER = 161,
	KEY_CLEARAGAIN = 162,
	KEY_CRSEL = 163,
	KEY_EXSEL = 164,

	KEY_KP_00 = 176,
	KEY_KP_000 = 177,
	KEY_THOUSANDSSEPARATOR = 178,
	KEY_DECIMALSEPARATOR = 179,
	KEY_CURRENCYUNIT = 180,
	KEY_CURRENCYSUBUNIT = 181,
	KEY_KP_LEFTPAREN = 182,
	KEY_KP_RIGHTPAREN = 183,
	KEY_KP_LEFTBRACE = 184,
	KEY_KP_RIGHTBRACE = 185,
	KEY_KP_TAB = 186,
	KEY_KP_BACKSPACE = 187,
	KEY_KP_A = 188,
	KEY_KP_B = 189,
	KEY_KP_C = 190,
	KEY_KP_D = 191,
	KEY_KP_E = 192,
	KEY_KP_F = 193,
	KEY_KP_XOR = 194,
	KEY_KP_POWER = 195,
	KEY_KP_PERCENT = 196,
	KEY_KP_LESS = 197,
	KEY_KP_GREATER = 198,
	KEY_KP_AMPERSAND = 199,
	KEY_KP_DBLAMPERSAND = 200,
	KEY_KP_VERTICALBAR = 201,
	KEY_KP_DBLVERTICALBAR = 202,
	KEY_KP_COLON = 203,
	KEY_KP_HASH = 204,
	KEY_KP_SPACE = 205,
	KEY_KP_AT = 206,
	KEY_KP_EXCLAM = 207,
	KEY_KP_MEMSTORE = 208,
	KEY_KP_MEMRECALL = 209,
	KEY_KP_MEMCLEAR = 210,
	KEY_KP_MEMADD = 211,
	KEY_KP_MEMSUBTRACT = 212,
	KEY_KP_MEMMULTIPLY = 213,
	KEY_KP_MEMDIVIDE = 214,
	KEY_KP_PLUSMINUS = 215,
	KEY_KP_CLEAR = 216,
	KEY_KP_CLEARENTRY = 217,
	KEY_KP_BINARY = 218,
	KEY_KP_OCTAL = 219,
	KEY_KP_DECIMAL = 220,
	KEY_KP_HEXADECIMAL = 221,

	KEY_LCTRL = 224,
	KEY_LSHIFT = 225,
	KEY_LALT = 226, /**< alt, option */
	KEY_LGUI = 227, /**< windows, command (apple), meta */
	KEY_RCTRL = 228,
	KEY_RSHIFT = 229,
	KEY_RALT = 230, /**< alt gr, option */
	KEY_RGUI = 231, /**< windows, command (apple), meta */

	KEY_MODE = 257,    /**< I'm not sure if this is really not covered
	of the above, but since there's a
	*   special KMOD_MODE for it I'm adding it here
	*/

	/*@}*//*Usage page 0x07*/

	/**
	*  \name Usage page 0x0C
	*
	*  These values are mapped from usage page 0x0C (USB consumer page).
	*/
	/*@{*/

	KEY_AUDIONEXT = 258,
	KEY_AUDIOPREV = 259,
	KEY_AUDIOSTOP = 260,
	KEY_AUDIOPLAY = 261,
	KEY_AUDIOMUTE = 262,
	KEY_MEDIASELECT = 263,
	KEY_WWW = 264,
	KEY_MAIL = 265,
	KEY_CALCULATOR = 266,
	KEY_COMPUTER = 267,
	KEY_AC_SEARCH = 268,
	KEY_AC_HOME = 269,
	KEY_AC_BACK = 270,
	KEY_AC_FORWARD = 271,
	KEY_AC_STOP = 272,
	KEY_AC_REFRESH = 273,
	KEY_AC_BOOKMARKS = 274,

	/*@}*//*Usage page 0x0C*/

	/**
	*  \name Walther keys
	*
	*  These are values that Christian Walther added (for mac keyboard?).
	*/
	/*@{*/

	KEY_BRIGHTNESSDOWN = 275,
	KEY_BRIGHTNESSUP = 276,
	KEY_DISPLAYSWITCH = 277, /**< display mirroring/dual display
							 switch, video mode switch */
	KEY_KBDILLUMTOGGLE = 278,
	KEY_KBDILLUMDOWN = 279,
	KEY_KBDILLUMUP = 280,
	KEY_EJECT = 281,
	KEY_SLEEP = 282, KEY_APP2 = 284,
} KEY;

#endif

class InputManager
{
public:
	InputManager();
	~InputManager();

public:
	//Initialize the input manager
	static void InitInputManager(WindowManager *window);
	//Get key down state
	static bool IsKeyDown(int key);
	//Get key released state
	static bool IsKeyUp(int key);

	//NOTE(kai): Callback is basically: once an event is fired store it in an event queue 
	//			,then use a callback function to process these events.
	//			Polling is basically going through the event queue at certain intervals 
	//			,and process these events (if there is any)

	//Process events using GLFW (Call back mode)
#if MODE_TYPE == GLFW_MODE
	static void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods);
	
	//Process events using SDL (Polling mode)
#elif MODE_TYPE == SDL_MODE
	static void Update(WindowManager *window);
#endif

private:
	//Array holding the key down state
	static bool m_KeysDown[512];
	//Array holding the key released state
	static bool m_KeysUp[512];

	static bool m_LastKeysDown[512];
#if MODE_TYPE == SDL_MODE
	static SDL_Event m_Event;	//Window events
#endif

};

