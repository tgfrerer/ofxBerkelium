#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxBerkelium.h"
#include "ofxBerkeliumUtil.h"
#include "ofxBerkeliumListener.h"


// NOTE:
// If you want to re-create this project,
// note all of the additions in the "Run Script" Build phase
// as well as the addional path in "Header Search Paths"
// ALSO
// check out the Window documentation for all of the stuff a Window can do
// http://berkelium.org/class_berkelium_1_1_window.html

typedef struct particle {
	ofPoint pos;
	ofPoint rot;
	ofPoint vel;
	float scale;
};

class testApp : public ofBaseApp, ofxBerkeliumListener {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void onAddressBarChanged(string newURL);
		void onStartLoading(string newURL);
		void onLoad();
		void onCrashedWorker();
		void onCrashedPlugin(wstring pluginName);
		void onProvisionalLoadError(string url, int errorCode, bool isMainFrame);
		void onConsoleMessage(wstring message, wstring sourceId, int line_no);
		void onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value);
		void onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction);
		void onLoadingStateChanged(bool isLoading);
		void onTitleChanged(wstring title);
		void onTooltipChanged(wstring text);
		void onCrashed();
		void onUnresponsive();
		void onResponsive();
		void onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags);
		void onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs);
		void onRunFileChooser(int mode, wstring title, string defaultFile);
		void onExternalHost(wstring message, string origin, string target);
	
		ofxBerkelium* browser;	
		vector<string> urls;
		bool loadRandomURL;
		bool drawLog;
	
		vector<particle> particles;
	
		vector<string> log;
};

#endif
