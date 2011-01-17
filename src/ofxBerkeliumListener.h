/*
 *  ofxBerkeliumListener.h
 *  Berkelium
 *
 *  Created by Jeffrey Crouse on 1/16/11.
 *  Copyright 2011 Eyebeam. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "berkelium/Berkelium.hpp"
#include "berkelium/ScriptUtil.hpp"


class ofxBerkeliumListener {
public:
	virtual void onAddressBarChanged(string newURL)=0;
    virtual void onStartLoading(string newURL)=0;
    virtual void onLoad()=0;
    virtual void onCrashedWorker()=0;
    virtual void onCrashedPlugin(wstring pluginName)=0;
    virtual void onProvisionalLoadError(string url, int errorCode, bool isMainFrame)=0;
    virtual void onConsoleMessage(wstring message, wstring sourceId, int line_no)=0;
    virtual void onScriptAlert(wstring message, wstring defaultValue, string url, int flags, bool success, wstring value)=0;
    virtual void onNavigationRequested(string newURL, string referrer, bool isNewWindow, bool &cancelDefaultAction)=0;
    virtual void onLoadingStateChanged(bool isLoading)=0;
    virtual void onTitleChanged(wstring title)=0;
    virtual void onTooltipChanged(wstring text)=0;
    virtual void onCrashed()=0;
    virtual void onUnresponsive()=0;
    virtual void onResponsive()=0;
    //virtual void onWidgetCreated(Berkelium::Widget *newWidget, int zIndex)=0;
    //virtual void onWidgetResize(Berkelium::Widget *wid, int newWidth, int newHeight)=0;
    //virtual void onWidgetMove(Berkelium::Widget *wid, int newX, int newY)=0;
    virtual void onShowContextMenu(int mouseX, int mouseY, int mediaType, string linkUrl, string srcUrl, string pageUrl, string frameUrl, wstring selectedText, bool isEditable, int editFlags)=0;
    virtual void onJavascriptCallback(void* replyMsg, string url, wstring funcName, Berkelium::Script::Variant *args, size_t numArgs)=0;
    virtual void onRunFileChooser(int mode, wstring title, string defaultFile)=0;
    virtual void onExternalHost(wstring message, string origin, string target)=0;
};

