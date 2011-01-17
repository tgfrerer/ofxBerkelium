#include "testApp.h"


#define WIDTH 800
#define HEIGHT 600
#define USE_TRANSPARENCY false


//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	
	Berkelium::init( Berkelium::FileString::empty() );

	browser = new ofxBerkelium(WIDTH, HEIGHT, USE_TRANSPARENCY);
	browser->window()->focus();
	browser->clear();
	browser->setListener(this);
	
	string url = "http://www.google.com";
	browser->getWindow()->navigateTo(url.data(), url.length());
}

//--------------------------------------------------------------
void testApp::update(){
	
	Berkelium::update();
	angle++;
}


//--------------------------------------------------------------
void testApp::draw(){
	
	ofPushMatrix();

	ofTranslate(100, ofGetHeight()/2.0, 0);
	ofRotateX(angle);
	ofTranslate(0, -ofGetHeight()/2.0, 0);
	
	
	browser->draw(0, 0);
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	int wvmods = mapGLUTModsToBerkeliumMods(glutGetModifiers());
	int vk_code = mapGLUTKeyToBerkeliumKey(key);
	int scancode = 0;
	cout << "key: " << key << " wvmods: " << wvmods << " vk_code: " << vk_code << " scancode: " << scancode << endl;
	browser->getWindow()->keyEvent(true, wvmods, vk_code, scancode);

	
	
	wchar_t outchars[2] = {(wchar_t)key};
	browser->getWindow()->textEvent(outchars, 1);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

	int wvmods = mapGLUTModsToBerkeliumMods(glutGetModifiers());
	int vk_code = mapGLUTKeyToBerkeliumKey(key);
	int scancode = 0;
	
	browser->getWindow()->keyEvent(false, wvmods, vk_code, scancode);

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	browser->window()->mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
        browser->window()->mouseButton( button, true );
    }
    else if (button == 3) {
        browser->window()->mouseWheel(0, 20);
    }
    else if (button == 4) {
        browser->window()->mouseWheel(0, -20);
    }

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    if (button == GLUT_LEFT_BUTTON || button == GLUT_MIDDLE_BUTTON || button == GLUT_RIGHT_BUTTON) {
        browser->window()->mouseButton( button, false );
    }
    else if (button == 3) {
        browser->window()->mouseWheel(0, 20);
    }
    else if (button == 4) {
       browser-> window()->mouseWheel(0, -20);
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


#pragma mark CALLBACKS


//--------------------------------------------------------------
void testApp::onAddressBarChanged(string newURL){
	cout << "*** onAddressBarChanged " << newURL << endl;
}

//--------------------------------------------------------------
void testApp::onStartLoading(string newURL){
	cout << "*** onStartLoading " << newURL << endl;
}

//--------------------------------------------------------------
void testApp::onLoad(){
	cout << "*** onLoad " << endl;
}

//--------------------------------------------------------------
void testApp::onCrashedWorker(){
	cout << "*** onCrashedWorker " << endl;
}

//--------------------------------------------------------------
void testApp::onCrashedPlugin(wstring pluginName){
	wcout << "*** onCrashedPlugin " << pluginName << endl;
}

//--------------------------------------------------------------
void testApp::onProvisionalLoadError(string url, int errorCode, bool isMainFrame){
	cout << "*** onProvisionalLoadError " << url << ": " << errorCode << (isMainFrame? " (main frame)":"") << endl;
}

//--------------------------------------------------------------
void testApp::onConsoleMessage(wstring message, wstring sourceId, int line_no){
	wcout << "*** onConsoleMessage " << message << " from " << sourceId << " line " << line_no << endl;
}

//--------------------------------------------------------------
void testApp::onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value){
	wcout << "*** onScriptAlert " << message << endl;
}

//--------------------------------------------------------------
void testApp::onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction){
	cout << "*** onNavigationRequested " << newURL << " by " << referrer << (isNewWindow?"  (new window)" : " (same window)") << endl;
}

//--------------------------------------------------------------
void testApp::onLoadingStateChanged(bool isLoading){
	cout << "*** onLoadingStateChanged " << (isLoading?"started":"stopped") << std::endl;
}

//--------------------------------------------------------------
void testApp::onTitleChanged(wstring title){
	wcout << "*** onTitleChanged " << title << endl;
	ofSetWindowTitle(wstring2string(title));
}

//--------------------------------------------------------------
void testApp::onTooltipChanged(wstring text){
	wcout << "*** onTooltipChanged " << text << endl;
}

//--------------------------------------------------------------
void testApp::onCrashed(){
	std::cout << "*** onCrashed " << std::endl;
}

//--------------------------------------------------------------
void testApp::onUnresponsive(){
	std::cout << "*** onUnresponsive " << std::endl;
}

//--------------------------------------------------------------
void testApp::onResponsive(){
	std::cout << "*** onResponsive " << std::endl;
}

//--------------------------------------------------------------
void testApp::onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags){
	std::cout << "*** onShowContextMenu at " << mouseX << "," << mouseY;
	std::cout << "; type: ";
	switch (mediaType) {
		case Berkelium::ContextMenuEventArgs::MediaTypeImage:
			cout << "image";
			break;
		case Berkelium::ContextMenuEventArgs::MediaTypeVideo:
			cout << "video";
			break;
		case Berkelium::ContextMenuEventArgs::MediaTypeAudio:
			cout << "audio";
			break;
		default:
			cout << "other";
	}
	if (isEditable || editFlags) {
		cout << " (";
		if (isEditable)
			cout << "editable; ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanUndo)
			cout << "Undo, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanRedo)
			cout << "Redo, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanCut)
			cout << "Cut, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanCopy)
			cout << "Copy, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanPaste)
			cout << "Paste, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanDelete)
			cout << "Delete, ";
		if (editFlags & Berkelium::ContextMenuEventArgs::CanSelectAll)
			cout << "Select All";
		std::cout << ")";
	}
	cout << endl;
	if (linkUrl.length())
		cout << "        Link URL: " << linkUrl << endl;
	if (srcUrl.length())
		cout << "        Source URL: " << srcUrl << endl;
	if (pageUrl.length())
		cout << "        Page URL: " << pageUrl << endl;
	if (frameUrl.length())
		cout << "        Frame URL: " << frameUrl << endl;
	if (selectedText.length())
		wcout << "        Selected Text: " << selectedText << endl;
}

//--------------------------------------------------------------
void testApp::onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs){
	
	std::cout << "*** onJavascriptCallback at URL " << url << ", " << (replyMsg?"synchronous":"async") << endl;
	
	wcout << "    Function name: " << funcName << endl;
	for (size_t i = 0; i < numArgs; i++) {
		Berkelium::WideString jsonStr = toJSON(args[i]);
		std::wcout << L"    Argument " << i << ": ";
		if (args[i].type() == Berkelium::Script::Variant::JSSTRING) {
			std::wcout << L"(string) " << args[i].toString() << std::endl;
		} else {
			std::wcout << jsonStr << std::endl;
		}
		Berkelium::Script::toJSON_free(jsonStr);
	}
	if (replyMsg) {
		browser->window()->synchronousScriptReturn(replyMsg, numArgs ? args[0] : Berkelium::Script::Variant());
	}
}

//--------------------------------------------------------------
void testApp::onRunFileChooser(int mode, wstring title, string defaultFile){
	wcout << "*** onRunFileChooser type " << mode << ", title " << title << ":" << endl;
	cout << defaultFile << std::endl;
	
	browser->window()->filesSelected(NULL);
}

//--------------------------------------------------------------
void testApp::onExternalHost(wstring message, string origin, string target){
	cout << "*** onExternalHost at URL from " << origin << " to " << target << ":" <<endl;
	wcout << message << endl;
}


