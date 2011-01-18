/*
 *  ofxBerkelium.cpp
 *  Berkelium
 *
 *  Created by Jeffrey Crouse on 1/16/11.
 *  Copyright 2011 Eyebeam. All rights reserved.
 *
 */

#include "ofxBerkelium.h"

//--------------------------------------------------------------
ofxBerkelium::ofxBerkelium(unsigned int _w, unsigned int _h, bool _usetrans) : width(_w), height(_h), needs_full_refresh(true)
{
	// Create texture to hold rendered view
	glGenTextures(1, &web_texture);
	glBindTexture(GL_TEXTURE_2D, web_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	
	scroll_buffer = new char[width*(height+1)*4];
	
	Berkelium::Context *context = Berkelium::Context::create();
	bk_window = Berkelium::Window::create(context);
	delete context;
	bk_window->setDelegate(this);
	bk_window->resize(width, height);
	bk_window->setTransparent(_usetrans);
	
	listener=NULL;
}

//--------------------------------------------------------------
ofxBerkelium::~ofxBerkelium() {
	delete scroll_buffer;
	delete bk_window;
}

//--------------------------------------------------------------
Berkelium::Window* ofxBerkelium::getWindow() {
	return bk_window;
}

//--------------------------------------------------------------
void ofxBerkelium::clear() {
	// Black out the page
	unsigned char black = 0;
	glBindTexture(GL_TEXTURE_2D, web_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 1, 1, 0,
				 GL_LUMINANCE, GL_UNSIGNED_BYTE, &black);
	
	needs_full_refresh = true;
}

//--------------------------------------------------------------
void ofxBerkelium::draw(float x, float y, float w, float h) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, web_texture);
	
	
	ofPushMatrix();
		ofTranslate(x, y);
		ofEnableNormalizedTexCoords();
			glBegin(GL_QUADS);
				glTexCoord2f(0.f, 0.f); glVertex3f(0, 0, 0.f);
				glTexCoord2f(0.f, 1.f); glVertex3f(0, h, 0.f);
				glTexCoord2f(1.f, 1.f); glVertex3f(w,  h, 0.f);
				glTexCoord2f(1.f, 0.f); glVertex3f(w, 0, 0.f);
			glEnd();
		ofDisableNormalizedTexCoords();
	ofPopMatrix();
	
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

//--------------------------------------------------------------
void ofxBerkelium::draw(float x, float y) {
	draw(x, y, width, height);
}

//--------------------------------------------------------------
void ofxBerkelium::setListener( ofxBerkeliumListener* _listener ) {
	listener = _listener;
}

//--------------------------------------------------------------
void ofxBerkelium::keyboard(int key, bool pressed) {
	
    // Some keys that come through this are actually special keys, despite being
    // represented as ASCII characters.
    if (key == '\b' || key == '\r' || key == '\n' || key == ' ' || key == 127 ||
        key >= 'a' && key <= 'z' || key >= 'A' && key <= 'Z') {
        int wvmods = mapGLUTModsToBerkeliumMods(glutGetModifiers());
        int vk_code = key == 127 ? BK_KEYCODE_DELETE : tolower(key);
        int scancode = 0;
		
        bk_window->keyEvent(pressed, wvmods, vk_code, scancode);
    }
	

	if (!isASCIISpecialToBerkelium(key) && !pressed) {
        // Regular text can be sent via textEvent
        wchar_t outchars[2];
        outchars[0] = key;
        outchars[1] = 0;
        bk_window->textEvent(outchars,1);
    }
}

//--------------------------------------------------------------
void ofxBerkelium::mouseMoved(int x, int y ) {
	
	bk_window->mouseMoved(x, y);
}


//--------------------------------------------------------------
void ofxBerkelium::mouseClick(int x, int y, int button, bool pressed ) {
	if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
        bk_window->mouseButton( button, pressed );
    }
    else if (button == 3) {
        bk_window->mouseWheel(0, 20);
    }
    else if (button == 4) {
        bk_window->mouseWheel(0, -20);
    }
}

#pragma mark CALLBACKS

void ofxBerkelium::onPaint(Berkelium::Window* wini,
					 const unsigned char* bitmap_in,
					 const Berkelium::Rect& bitmap_rect,
					 size_t num_copy_rects,
					 const Berkelium::Rect* copy_rects,
					 int dx,  int dy,
					 const Berkelium::Rect&	scroll_rect ) {
	//std::cout << "*** onPaint" << std::endl;
	bool updated = mapOnPaintToTexture(wini, bitmap_in, bitmap_rect, num_copy_rects, copy_rects,
									   dx, dy, scroll_rect,
									   needs_full_refresh);
	if (updated) {
		needs_full_refresh = false;
	}
}

void ofxBerkelium::onAddressBarChanged(Berkelium::Window *win, Berkelium::URLString newURL) {
	if(listener) listener->onAddressBarChanged(newURL.data());
}
void ofxBerkelium::onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL) {
	if(listener) listener->onStartLoading(newURL.data());
}
void ofxBerkelium::onLoad(Berkelium::Window *win) {
	if(listener) listener->onLoad();
}
void ofxBerkelium::onCrashedWorker(Berkelium::Window *win) {
	if(listener) listener->onCrashedWorker();
}
void ofxBerkelium::onCrashedPlugin(Berkelium::Window *win, Berkelium::WideString pluginName) {
	if(listener) listener->onCrashedPlugin(pluginName.data());
}
void ofxBerkelium::onProvisionalLoadError(Berkelium::Window *win, Berkelium::URLString url,
									int errorCode, bool isMainFrame) {
	if(listener) listener->onProvisionalLoadError(url.data(), errorCode, isMainFrame);
}
void ofxBerkelium::onConsoleMessage(Berkelium::Window *win, Berkelium::WideString message,
							  Berkelium::WideString sourceId, int line_no) {
	if(listener) listener->onConsoleMessage(message.data(), sourceId.data(), line_no);
}
void ofxBerkelium::onScriptAlert(Berkelium::Window *win, Berkelium::WideString message,
						   Berkelium::WideString defaultValue, Berkelium::URLString url,
						   int flags, bool &success, Berkelium::WideString &value) {
	if(listener) listener->onScriptAlert(message.data(), defaultValue.data(), url.data(), 
										 flags, success, value.data());
}
void ofxBerkelium::onNavigationRequested(Berkelium::Window *win, Berkelium::URLString newURL,
								   Berkelium::URLString referrer, bool isNewWindow,
								   bool &cancelDefaultAction) {
	if(listener) listener->onNavigationRequested(newURL.data(), referrer.data(), isNewWindow, cancelDefaultAction);
}
void ofxBerkelium::onLoadingStateChanged(Berkelium::Window *win, bool isLoading) {
	if(listener) listener->onLoadingStateChanged(isLoading);
}
void ofxBerkelium::onTitleChanged(Berkelium::Window *win, Berkelium::WideString title) {
	if(listener) listener->onTitleChanged(title.data());
}
void ofxBerkelium::onTooltipChanged(Berkelium::Window *win, Berkelium::WideString text) {
	if(listener) listener->onTooltipChanged(text.data());
}
void ofxBerkelium::onCrashed(Berkelium::Window *win) {
	if(listener) listener->onCrashed();
}
void ofxBerkelium::onUnresponsive(Berkelium::Window *win) {
	if(listener) listener->onUnresponsive();
}
void ofxBerkelium::onResponsive(Berkelium::Window *win) {
	if(listener) listener->onResponsive();
}
void ofxBerkelium::onCreatedWindow(Berkelium::Window *win, Berkelium::Window *newWindow, const Berkelium::Rect &initialRect) {
	std::cout << "*** onCreatedWindow "
	<< initialRect.mLeft << "," << initialRect.mTop << ": "
	<< initialRect.mWidth << "x" << initialRect.mHeight << std::endl;
}
void ofxBerkelium::onWidgetCreated(Berkelium::Window *win, Berkelium::Widget *newWidget, int zIndex) {
	std::cout << "*** onWidgetCreated " << newWidget << " index " << zIndex << std::endl;
}
void ofxBerkelium::onWidgetResize(Berkelium::Window *win, Berkelium::Widget *wid, int newWidth, int newHeight) {
	std::cout << "*** onWidgetResize " << wid << " "
	<< newWidth << "x" << newHeight << std::endl;
}
void ofxBerkelium::onWidgetMove(Berkelium::Window *win, Berkelium::Widget *wid, int newX, int newY) {
	std::cout << "*** onWidgetMove " << wid << " "
	<< newX << "," << newY << std::endl;
}
void ofxBerkelium::onShowContextMenu(Berkelium::Window *win,
							   const Berkelium::ContextMenuEventArgs& args) {
	
	if(listener) listener->onShowContextMenu(args.mouseX, args.mouseY, args.mediaType, args.linkUrl.data(), 
											 args.srcUrl.data(), args.pageUrl.data(), args.frameUrl.data(), 
											 args.selectedText.data(), args.isEditable, args.editFlags);
}

void ofxBerkelium::onJavascriptCallback(Berkelium::Window *win, void* replyMsg, Berkelium::URLString url, 
										Berkelium::WideString funcName, Berkelium::Script::Variant *args, 
										size_t numArgs) {
	if(listener) listener->onJavascriptCallback(replyMsg, url.data(), funcName.data(), args, numArgs);
}

void ofxBerkelium::onRunFileChooser(Berkelium::Window *win, int mode, Berkelium::WideString title, 
									Berkelium::FileString defaultFile) {
	if(listener) listener->onRunFileChooser(mode, title.data(), defaultFile.data());
}

void ofxBerkelium::onExternalHost(Berkelium::Window *win, Berkelium::WideString message, Berkelium::URLString origin, 
								  Berkelium::URLString target) {
	if(listener) listener->onExternalHost(message.data(), origin.data(), target.data());
}



/** Handles an onPaint call by mapping the results into an OpenGL texture. The
 *  first parameters are the same as Berkelium::WindowDelegate::onPaint.  The
 *  additional parameters and return value are:
 *  \param dest_texture - the OpenGL texture handle for the texture to render
 *                        the results into.
 *  \param dest_texture_width - width of destination texture
 *  \param dest_texture_height - height of destination texture
 *  \param ignore_partial - if true, ignore any partial updates.  This is useful
 *         if you have loaded a new page, but updates for the old page have not
 *         completed yet.
 *  \param scroll_buffer - a temporary workspace used for scroll data.  Must be
 *         at least dest_texture_width * dest_texture_height * 4 bytes large.
 *  \returns true if the texture was updated, false otherwiase
 */
bool ofxBerkelium::mapOnPaintToTexture(
						 Berkelium::Window *wini,
						 const unsigned char* bitmap_in, const Berkelium::Rect& bitmap_rect,
						 size_t num_copy_rects, const Berkelium::Rect *copy_rects,
						 int dx, int dy,
						 const Berkelium::Rect& scroll_rect,
						 bool ignore_partial) {
	
    glBindTexture(GL_TEXTURE_2D, web_texture);
	
    const int kBytesPerPixel = 4;
	
    // If we've reloaded the page and need a full update, ignore updates
    // until a full one comes in.  This handles out of date updates due to
    // delays in event processing.
    if (ignore_partial) {
        if (bitmap_rect.left() != 0 ||
            bitmap_rect.top() != 0 ||
            bitmap_rect.right() != width ||
            bitmap_rect.bottom() != height) {
            return false;
        }
		
        glTexImage2D(GL_TEXTURE_2D, 0, kBytesPerPixel, width, height, 0,
					 GL_BGRA, GL_UNSIGNED_BYTE, bitmap_in);
        ignore_partial = false;
        return true;
    }
	
	
    // Now, we first handle scrolling. We need to do this first since it
    // requires shifting existing data, some of which will be overwritten by
    // the regular dirty rect update.
    if (dx != 0 || dy != 0) {
        // scroll_rect contains the Rect we need to move
        // First we figure out where the the data is moved to by translating it
        Berkelium::Rect scrolled_rect = scroll_rect.translate(-dx, -dy);
        // Next we figure out where they intersect, giving the scrolled
        // region
        Berkelium::Rect scrolled_shared_rect = scroll_rect.intersect(scrolled_rect);
        // Only do scrolling if they have non-zero intersection
        if (scrolled_shared_rect.width() > 0 && scrolled_shared_rect.height() > 0) {
            // And the scroll is performed by moving shared_rect by (dx,dy)
            Berkelium::Rect shared_rect = scrolled_shared_rect.translate(dx, dy);
			
            int wid = scrolled_shared_rect.width();
            int hig = scrolled_shared_rect.height();
            if (DEBUG_PAINT) {
				std::cout << "Scroll rect: w=" << wid << ", h=" << hig << ", ("
				<< scrolled_shared_rect.left() << "," << scrolled_shared_rect.top()
				<< ") by (" << dx << "," << dy << ")" << std::endl;
            }
            int inc = 1;
            char *outputBuffer = scroll_buffer;
            // source data is offset by 1 line to prevent memcpy aliasing
            // In this case, it can happen if dy==0 and dx!=0.
            char *inputBuffer = scroll_buffer+(width*1*kBytesPerPixel);
            int jj = 0;
            if (dy > 0) {
                // Here, we need to shift the buffer around so that we start in the
                // extra row at the end, and then copy in reverse so that we
                // don't clobber source data before copying it.
                outputBuffer = scroll_buffer+(
											  (scrolled_shared_rect.top()+hig+1)*width
											  - hig*wid)*kBytesPerPixel;
                inputBuffer = scroll_buffer;
                inc = -1;
                jj = hig-1;
            }
			
            // Copy the data out of the texture
            glGetTexImage(
						  GL_TEXTURE_2D, 0,
						  GL_BGRA, GL_UNSIGNED_BYTE,
						  inputBuffer
						  );
			
            // Annoyingly, OpenGL doesn't provide convenient primitives, so
            // we manually copy out the region to the beginning of the
            // buffer
            for(; jj < hig && jj >= 0; jj+=inc) {
                memcpy(
					   outputBuffer + (jj*wid) * kBytesPerPixel,
					   //scroll_buffer + (jj*wid * kBytesPerPixel),
					   inputBuffer + (
									  (scrolled_shared_rect.top()+jj)*width
									  + scrolled_shared_rect.left()) * kBytesPerPixel,
					   wid*kBytesPerPixel
					   );
            }
			
            // And finally, we push it back into the texture in the right
            // location
            glTexSubImage2D(GL_TEXTURE_2D, 0,
							shared_rect.left(), shared_rect.top(),
							shared_rect.width(), shared_rect.height(),
							GL_BGRA, GL_UNSIGNED_BYTE, outputBuffer
							);
        }
    }
	
    if (DEBUG_PAINT) {
		std::cout << "Bitmap rect: w="
		<< bitmap_rect.width()<<", h="<<bitmap_rect.height()
		<<", ("<<bitmap_rect.top()<<","<<bitmap_rect.left()
		<<") tex size "<<width<<"x"<<height
		<<std::endl;
    }
    for (size_t i = 0; i < num_copy_rects; i++) {
        int wid = copy_rects[i].width();
        int hig = copy_rects[i].height();
        int top = copy_rects[i].top() - bitmap_rect.top();
        int left = copy_rects[i].left() - bitmap_rect.left();
        if (DEBUG_PAINT) {
            std::cout << "Copy rect: w=" << wid << ", h=" << hig << ", ("
			<< top << "," << left << ")" << std::endl;
        }
        for(int jj = 0; jj < hig; jj++) {
            memcpy(
				   scroll_buffer + jj*wid*kBytesPerPixel,
				   bitmap_in + (left + (jj+top)*bitmap_rect.width())*kBytesPerPixel,
				   wid*kBytesPerPixel
				   );
        }
		
        // Finally, we perform the main update, just copying the rect that is
        // marked as dirty but not from scrolled data.
        glTexSubImage2D(GL_TEXTURE_2D, 0,
                        copy_rects[i].left(), copy_rects[i].top(),
                        wid, hig,
                        GL_BGRA, GL_UNSIGNED_BYTE, scroll_buffer
						);
    }
	
    glBindTexture(GL_TEXTURE_2D, 0);
	
    return true;
}