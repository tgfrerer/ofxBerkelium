#include "testApp.h"


#define WIDTH 1024
#define HEIGHT 768
#define USE_TRANSPARENCY false


//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	
	Berkelium::init( Berkelium::FileString::empty() );

	browser = new ofxBerkelium(WIDTH, HEIGHT, USE_TRANSPARENCY);
	browser->setListener(this);
	
	urls.push_back("http://www.jeffcrouse.info/how-tos/ofxberkelium");
	urls.push_back("https://github.com/jefftimesten/ofxBerkelium");
	urls.push_back("http://www.openframeworks.cc");
	urls.push_back("http://berkelium.org/");
	urls.push_back("http://www.cnn.com");
	urls.push_back("http://vimeo.com/18532317");
	urls.push_back("http://vimeo.com/18786679");
	
	for(int i=0; i<10; i++) {
		particle p;
		p.pos.x = ofRandomWidth();
		p.pos.y = ofRandomHeight();
		p.vel.x = ofRandom(-10, 10);
		p.vel.y = ofRandom(-10, 10);
		p.rot.x = ofRandom(0, 360);
		p.rot.y = ofRandom(0, 360);
		p.rot.z = ofRandom(0, 360);
		p.scale = .02 + ofRandomf()  * .1;
		particles.push_back( p );
	}
	
	loadRandomURL=true;
	drawLog = false;
}

//--------------------------------------------------------------
void testApp::update(){
	
	Berkelium::update();
	
	if(loadRandomURL) {
		int n = ofRandom(0, urls.size());
		browser->getWindow()->navigateTo(urls[n].data(), urls[n].length());
		loadRandomURL=false;
	}
	
	for(int i=0; i<particles.size(); i++) {
		particles[i].pos += particles[i].vel;
		particles[i].rot += ofPoint(1,1,1);
		
		if(particles[i].pos.x > ofGetWidth()) {
			particles[i].pos.x = ofGetWidth();
			particles[i].vel.x *= -1;
		}
		if(particles[i].pos.y > ofGetHeight()) {
			particles[i].pos.y = ofGetHeight();
			particles[i].vel.y *= -1;
		}
		if(particles[i].pos.x < 0) {
			particles[i].pos.x = 0;
			particles[i].vel.x *= -1;
		}
		if(particles[i].pos.y < 0) {
			particles[i].pos.y = 0;
			particles[i].vel.y *= -1;
		}
	}
}


//--------------------------------------------------------------
void testApp::draw(){
	
	browser->draw(0, 0);
	

	for(int i=0; i<particles.size(); i++)
	{
		ofPushMatrix();
		ofTranslate(particles[i].pos.x, particles[i].pos.y);
		ofRotateX(particles[i].rot.x);
		ofRotateY(particles[i].rot.y);
		ofRotateZ(particles[i].rot.z);
		ofScale(particles[i].scale, particles[i].scale, 1);
		browser->draw(0,0);
		ofPopMatrix();
	}
	
	ofPushStyle();
	
	if(drawLog)
	{
		ofEnableAlphaBlending();
		ofSetColor(255, 255, 255, 180);
		ofRect(10, 10, 600, ofGetHeight()-10);
		ofSetColor(0, 0, 0, 255);
		int y=30;
		for(int i=0; i<log.size(); i++) {
			ofDrawBitmapString(log[i], 20, y);
			y+=16;
		}
		ofDisableAlphaBlending();
	}
	
	if(browser->status=="") {
		browser->status = "Press right key to load another URL";
	}
	ofSetColor(0x000000);
	ofRect(0, ofGetHeight()-25, ofGetWidth(), 25);
	ofSetColor(0xFFFFFF);
	ofDrawBitmapString(browser->status, 10, ofGetHeight()-7);
	ofPopStyle();
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	browser->keyboard(key, true);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
	switch(key) {
		case OF_KEY_RIGHT:
			loadRandomURL=true;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 'l':
			drawLog = !drawLog;
			break;
	}
		
	browser->keyboard(key, false);
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	browser->mouseMoved(x, y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

	browser->mouseClick(x, y, button, true);

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	browser->mouseClick(x, y, button, false);
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}


#pragma mark CALLBACKS


//--------------------------------------------------------------
void testApp::onAddressBarChanged(string newURL){
	cout << "*** onAddressBarChanged " << newURL << endl;
	log.push_back("*** onAddressBarChanged "+newURL);
}

//--------------------------------------------------------------
void testApp::onStartLoading(string newURL){
	cout << "*** onStartLoading " << newURL << endl;
	log.push_back("*** onStartLoading " +newURL);
}

//--------------------------------------------------------------
void testApp::onLoad(){
	cout << "*** onLoad " << endl;
	log.push_back("*** onLoad ");
}

//--------------------------------------------------------------
void testApp::onCrashedWorker(){
	cout << "*** onCrashedWorker " << endl;
	log.push_back("*** onCrashedWorker ");
}

//--------------------------------------------------------------
void testApp::onCrashedPlugin(wstring pluginName){
	wcout << "*** onCrashedPlugin " << pluginName << endl;
	log.push_back("*** onCrashedPlugin ");
}

//--------------------------------------------------------------
void testApp::onProvisionalLoadError(string url, int errorCode, bool isMainFrame){
	cout << "*** onProvisionalLoadError " << url << ": " << errorCode << (isMainFrame? " (main frame)":"") << endl;
	log.push_back("*** onProvisionalLoadError " );
}

//--------------------------------------------------------------
void testApp::onConsoleMessage(wstring message, wstring sourceId, int line_no){
	wcout << "*** onConsoleMessage " << message << " from " << sourceId << " line " << line_no << endl;
	log.push_back("*** onConsoleMessage " + wstring2string(message));
}

//--------------------------------------------------------------
void testApp::onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value){
	wcout << "*** onScriptAlert " << message << endl;
	log.push_back("*** onScriptAlert ");
}

//--------------------------------------------------------------
void testApp::onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction){
	cout << "*** onNavigationRequested " << newURL << " by " << referrer << (isNewWindow?"  (new window)" : " (same window)") << endl;
	log.push_back("*** onNavigationRequested " + newURL);
}

//--------------------------------------------------------------
void testApp::onLoadingStateChanged(bool isLoading){
	cout << "*** onLoadingStateChanged " << (isLoading?"started":"stopped") << std::endl;
	log.push_back("*** onLoadingStateChanged ");
}

//--------------------------------------------------------------
void testApp::onTitleChanged(wstring title){
	wcout << "*** onTitleChanged " << title << endl;
	ofSetWindowTitle(wstring2string(title));
	log.push_back("*** onTitleChanged ");
}

//--------------------------------------------------------------
void testApp::onTooltipChanged(wstring text){
	wcout << "*** onTooltipChanged " << text << endl;
	log.push_back("*** onTooltipChanged ");
}

//--------------------------------------------------------------
void testApp::onCrashed(){
	cout << "*** onCrashed " << endl;
	log.push_back("*** onCrashed ");
}

//--------------------------------------------------------------
void testApp::onUnresponsive(){
	cout << "*** onUnresponsive " << endl;
	log.push_back("*** onUnresponsive ");
}

//--------------------------------------------------------------
void testApp::onResponsive(){
	cout << "*** onResponsive " << endl;
	log.push_back("*** onResponsive ");
}

//--------------------------------------------------------------
void testApp::onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags){
	cout << "*** onShowContextMenu at " << mouseX << "," << mouseY;
	cout << "; type: ";
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
		cout << ")";
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
	
	log.push_back("*** onShowContextMenu");
}

//--------------------------------------------------------------
void testApp::onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs){
	
	cout << "*** onJavascriptCallback at URL " << url << ", " << (replyMsg?"synchronous":"async") << endl;
	
	wcout << "    Function name: " << funcName << endl;
	for (size_t i = 0; i < numArgs; i++) {
		Berkelium::WideString jsonStr = toJSON(args[i]);
		wcout << L"    Argument " << i << ": ";
		if (args[i].type() == Berkelium::Script::Variant::JSSTRING) {
			wcout << L"(string) " << args[i].toString() << endl;
		} else {
			wcout << jsonStr << endl;
		}
		Berkelium::Script::toJSON_free(jsonStr);
	}
	if (replyMsg) {
		browser->window()->synchronousScriptReturn(replyMsg, numArgs ? args[0] : Berkelium::Script::Variant());
	}
	
	log.push_back("*** onJavascriptCallback at URL " + url);
}

//--------------------------------------------------------------
void testApp::onRunFileChooser(int mode, wstring title, string defaultFile){
	wcout << "*** onRunFileChooser type " << mode << ", title " << title << ":" << endl;
	cout << defaultFile << endl;
	
	browser->window()->filesSelected(NULL);
}

//--------------------------------------------------------------
void testApp::onExternalHost(wstring message, string origin, string target){
	cout << "*** onExternalHost at URL from " << origin << " to " << target << ":" <<endl;
	wcout << message << endl;
}


