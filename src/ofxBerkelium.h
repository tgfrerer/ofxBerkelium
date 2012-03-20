/*
 *  ofxBerkelium.h
 *  Berkelium
 *
 *  Created by Jeffrey Crouse on 1/16/11.
 *  Copyright 2011 Eyebeam. All rights reserved.
 *
 */

#include "ofMain.h"
#include "berkelium/Berkelium.hpp"
#include "berkelium/Window.hpp"
#include "berkelium/WindowDelegate.hpp"
#include "berkelium/Context.hpp"
#include "berkelium/ScriptUtil.hpp"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"

#define DEBUG_PAINT false

/** ofxBerkelium handles rendering a window into a GL texture.  Unlike the
 *  utility methods, this takes care of the entire process and cleanup.
 */
class ofxBerkelium : Berkelium::WindowDelegate {
public:
    ofxBerkelium(unsigned int _w, unsigned int _h, bool _usetrans);
    ~ofxBerkelium();

	void setListener( ofxBerkeliumListener* listener );
    Berkelium::Window* getWindow();
    Berkelium::Window* window() const {
        return bk_window;
    }
    void clear();
	void draw(float x, float y);
	void draw(float x, float y, float w, float h);

	void keyboard(int key, bool pressed);
	void mouseMoved(int x, int y );
	void mouseClick(int x, int y, int button, bool pressed);

	unsigned char* getPixels() {
		// TO DO:  Implement this, preferably without getting the pixels out of the texture (too slow?)
	}

    void executeJavaScript(std::string& script){
        Berkelium::WideString tmpString;

        std::wstring widestring = string2wstring(script);

        tmpString = Berkelium::WideString::point_to(widestring);

        bk_window->executeJavascript(tmpString);

    }

	void back() {
		bk_window->goBack();
	}

	void forward() {
		bk_window->goForward();
	}

	string status;
	string title;
	string addressBar;

#pragma mark CALLBACKS

	void onPaint(Berkelium::Window* wini, const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect, size_t num_copy_rects, const Berkelium::Rect* copy_rects, int dx, int dy, const Berkelium::Rect& scroll_rect);
    void onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL);
    void onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL);
    void onLoad(Berkelium::Window *win);
    void onCrashedWorker(Berkelium::Window *win);
    void onCrashedPlugin(Berkelium::Window *win, Berkelium::WideString pluginName);
    void onProvisionalLoadError(Berkelium::Window *win, Berkelium::URLString url, int errorCode, bool isMainFrame);
    void onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString sourceId, int line_no);
    void onScriptAlert(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool &success, Berkelium::WideString &value);
    void onNavigationRequested(Berkelium::Window *win, Berkelium::URLString newURL, Berkelium::URLString referrer, bool isNewWindow, bool &cancelDefaultAction);
    void onLoadingStateChanged(Berkelium::Window *win, bool isLoading);
    void onTitleChanged(Berkelium::Window *win, Berkelium::WideString title);
    void onTooltipChanged(Berkelium::Window *win, Berkelium::WideString text);
    void onCrashed(Berkelium::Window *win);
    void onUnresponsive(Berkelium::Window *win);
    void onResponsive(Berkelium::Window *win);
    void onCreatedWindow(Berkelium::Window *win, Berkelium::Window *newWindow, const Berkelium::Rect &initialRect);
    void onWidgetCreated(Berkelium::Window *win, Berkelium::Widget *newWidget, int zIndex);
    void onWidgetResize(Berkelium::Window *win, Berkelium::Widget *wid, int newWidth, int newHeight);
    void onWidgetMove(Berkelium::Window *win, Berkelium::Widget *wid, int newX, int newY);
    void onShowContextMenu(Berkelium::Window *win, const Berkelium::ContextMenuEventArgs& args) ;
    void onJavascriptCallback(Berkelium::Window *win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, Berkelium::Script::Variant *args, size_t numArgs);
    void onRunFileChooser(Berkelium::Window *win, int mode, Berkelium::WideString title, Berkelium::FileString defaultFile);
    void onExternalHost(Berkelium::Window *win, Berkelium::WideString message, Berkelium::URLString origin, Berkelium::URLString target);


private:
	bool mapOnPaintToTexture(Berkelium::Window *wini,
							 const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
							 size_t num_copy_rects, const Berkelium::Rect *copy_rects,
							 int dx, int dy,
							 const Berkelium::Rect& scroll_rect,
							 bool ignore_partial);

    // The Berkelium window, i.e. our web page
    Berkelium::Window* bk_window;
    // Width and height of our window.
    unsigned int width, height;
    // Storage for a texture
    unsigned int web_texture;
    // Bool indicating when we need to refresh the entire image
    bool needs_full_refresh;
    // Buffer used to store data for scrolling
    char* scroll_buffer;
	// The testApp (or some other delagate class) that receives events from the browser
	ofxBerkeliumListener* listener;

};
