/*
 *  ofxBerkeliumUtil.h
 *  Berkelium
 *
 *  Created by Jeffrey Crouse on 1/16/11.
 *  Copyright 2011 Eyebeam. All rights reserved.
 *
 */

#pragma once

#ifdef TARGET_WIN32
	#define GLUT_BUILDING_LIB
	#include "glut.h"
#endif
#ifdef TARGET_OSX
	#include "../../../libs/glut/lib/osx/GLUT.framework/Versions/A/Headers/glut.h"
	#include <Carbon/Carbon.h>
#endif
#ifdef TARGET_LINUX
	#include <GL/glut.h>
#endif

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


/** tig: The follwing string2wstring method is based on:
 *       <http://altctrlbackspace.blogspot.com/2009/03/c-utf8-to-wstring-conversion-routine.html>
 *
 *  Copyright (c) 2009 SegFault aka "ErV" (altctrlbackspace.blogspot.com)
 *
 *  Redistribution and use of this source code, with or without modification, is
 *  permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 *  EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

inline std::wstring string2wstring(const string& src){
     wstring dest;

     dest.clear();
     wchar_t w = 0;
     int bytes = 0;
     wchar_t err = L'?';
     for (size_t i = 0; i < src.size(); i++){
      unsigned char c = (unsigned char)src[i];
      if (c <= 0x7f){//first byte
       if (bytes){
        dest.push_back(err);
        bytes = 0;
       }
       dest.push_back((wchar_t)c);
      }
      else if (c <= 0xbf){//second/third/etc byte
       if (bytes){
        w = ((w << 6)|(c & 0x3f));
        bytes--;
        if (bytes == 0)
         dest.push_back(w);
       }
       else
        dest.push_back(err);
      }
      else if (c <= 0xdf){//2byte sequence start
       bytes = 1;
       w = c & 0x1f;
      }
      else if (c <= 0xef){//3byte sequence start
       bytes = 2;
       w = c & 0x0f;
      }
      else if (c <= 0xf7){//3byte sequence start
       bytes = 3;
       w = c & 0x07;
      }
      else{
       dest.push_back(err);
       bytes = 0;
      }
     }
     if (bytes)
      dest.push_back(err);


     return dest;
}
