/*
 *  ofxBerkeliumUtil.h
 *  Berkelium
 *
 *  Created by Jeffrey Crouse on 1/16/11.
 *  Copyright 2011 Eyebeam. All rights reserved.
 *
 */

#pragma once

/** Given modifiers retrieved from GLUT (e.g. glutGetModifiers), convert to a
 *  form that can be passed to Berkelium.
 */
inline int mapGLUTModsToBerkeliumMods(int modifiers) {
    int wvmods = 0;
	
    if (modifiers & GLUT_ACTIVE_SHIFT)
        wvmods |= Berkelium::SHIFT_MOD;
    if (modifiers & GLUT_ACTIVE_CTRL)
        wvmods |= Berkelium::CONTROL_MOD;
    if (modifiers & GLUT_ACTIVE_ALT)
        wvmods |= Berkelium::ALT_MOD;
	
    // Note: GLUT doesn't expose Berkelium::META_MOD
	
    return wvmods;
}

/** Returns true if the ASCII value is considered a special input to Berkelium
 *  which cannot be handled directly via the textEvent method and must be
 *  handled using keyEvent instead.
 */
inline unsigned int isASCIISpecialToBerkelium(unsigned int glut_char) {
    unsigned char ASCII_BACKSPACE = 8;
    unsigned char ASCII_TAB       = 9;
    unsigned char ASCII_ESCAPE    = 27;
    unsigned char ASCII_DELETE    = 127;
	
    return (glut_char == ASCII_BACKSPACE ||
			glut_char == ASCII_TAB ||
			glut_char == ASCII_ESCAPE ||
			glut_char == ASCII_DELETE
			);
}

// A few of the most useful keycodes handled below.
enum VirtKeys {
	BK_KEYCODE_PRIOR = 0x21,
	BK_KEYCODE_NEXT = 0x22,
	BK_KEYCODE_END = 0x23,
	BK_KEYCODE_HOME = 0x24,
	BK_KEYCODE_LEFT = 0x25,
	BK_KEYCODE_UP = 0x26,
	BK_KEYCODE_RIGHT = 0x27,
	BK_KEYCODE_DOWN = 0x28,
	BK_KEYCODE_INSERT = 0x2D,
	BK_KEYCODE_DELETE = 0x2E
	
};

/** Given an input key from GLUT, convert it to a form that can be passed to
 *  Berkelium.
 */
inline unsigned int mapGLUTKeyToBerkeliumKey(int glut_key) {
    switch(glut_key) {
#define MAP_VK(X, Y) case GLUT_KEY_##X: return BK_KEYCODE_##Y;
			MAP_VK(INSERT, INSERT);
			MAP_VK(HOME, HOME);
			MAP_VK(END, END);
			MAP_VK(PAGE_UP, PRIOR);
			MAP_VK(PAGE_DOWN, NEXT);
			MAP_VK(LEFT, LEFT);
			MAP_VK(RIGHT, RIGHT);
			MAP_VK(UP, UP);
			MAP_VK(DOWN, DOWN);
		default: return 0;
    }
}

/**
 *	FIXME:  I've been told this isn't portable
 */
inline string wstring2string(const wstring& ws) {
	string ns(ws.begin(), ws.end());
    return  ns;
}
