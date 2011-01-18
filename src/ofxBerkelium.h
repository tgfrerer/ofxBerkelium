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

#pragma mark CALLBACKS
    virtual void onPaint(Berkelium::Window* wini, const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect, size_t num_copy_rects, const Berkelium::Rect* copy_rects, int dx, int dy, const Berkelium::Rect& scroll_rect);
    virtual void onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL);
    virtual void onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL);
    virtual void onLoad(Berkelium::Window *win);
    virtual void onCrashedWorker(Berkelium::Window *win);
    virtual void onCrashedPlugin(Berkelium::Window *win, Berkelium::WideString pluginName);
    virtual void onProvisionalLoadError(Berkelium::Window *win, Berkelium::URLString url, int errorCode, bool isMainFrame);
    virtual void onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString sourceId, int line_no);
    virtual void onScriptAlert(Berkelium::Window *win, Berkelium::WideString message, Berkelium::WideString defaultValue, Berkelium::URLString url, int flags, bool &success, Berkelium::WideString &value);
    virtual void onNavigationRequested(Berkelium::Window *win, Berkelium::URLString newURL, Berkelium::URLString referrer, bool isNewWindow, bool &cancelDefaultAction);
    virtual void onLoadingStateChanged(Berkelium::Window *win, bool isLoading);
    virtual void onTitleChanged(Berkelium::Window *win, Berkelium::WideString title);
    virtual void onTooltipChanged(Berkelium::Window *win, Berkelium::WideString text);
    virtual void onCrashed(Berkelium::Window *win);
    virtual void onUnresponsive(Berkelium::Window *win);
    virtual void onResponsive(Berkelium::Window *win);
    virtual void onCreatedWindow(Berkelium::Window *win, Berkelium::Window *newWindow, const Berkelium::Rect &initialRect);
    virtual void onWidgetCreated(Berkelium::Window *win, Berkelium::Widget *newWidget, int zIndex);
    virtual void onWidgetResize(Berkelium::Window *win, Berkelium::Widget *wid, int newWidth, int newHeight);
    virtual void onWidgetMove(Berkelium::Window *win, Berkelium::Widget *wid, int newX, int newY);
    virtual void onShowContextMenu(Berkelium::Window *win, const Berkelium::ContextMenuEventArgs& args) ;
    virtual void onJavascriptCallback(Berkelium::Window *win, void* replyMsg, Berkelium::URLString url, Berkelium::WideString funcName, Berkelium::Script::Variant *args, size_t numArgs);
    virtual void onRunFileChooser(Berkelium::Window *win, int mode, Berkelium::WideString title, Berkelium::FileString defaultFile);
    virtual void onExternalHost(Berkelium::Window *win, Berkelium::WideString message, Berkelium::URLString origin, Berkelium::URLString target);

	
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