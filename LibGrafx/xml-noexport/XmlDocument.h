// FiducialDialog.cpp : implementation file
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#pragma once
//! \cond IGNORE

#include <string>
#include <list>

//
// class CXmlDocument
// General purpose class for managing an XML document in memory.
// Allows for reading and writing documents.
//

class CXmlDocument
{
public:
    CXmlDocument(void);
    virtual ~CXmlDocument(void);

    // File reading operations
    virtual bool OpenXmlDocument(const wchar_t *fileName, const wchar_t *rootName);

    // File writing operations
    bool CreateXmlDocument(const wchar_t *rootName);
    bool WriteXmlDocument(const wchar_t *fileName);

    virtual IXMLDOMNode *GetRootNode() {return mRootnode;}

    // Element management
    virtual void PushElement(const wchar_t *elementName);
    virtual void PopElement();
    IXMLDOMElement *GetCurrentElement() {return mElementsStack.front().mElement;}
    IXMLDOMNode *GetCurrentNode() {return mElementsStack.front().mNode;}

    // Attribute management
    virtual void SetAttribute(const wchar_t *name, const wchar_t *value);
    virtual void SetAttribute(const wchar_t *name, int value);
    virtual void SetAttribute(const wchar_t *name, double value);
    virtual void SetAttribute(const wchar_t *name, bool value);

    virtual const wchar_t *GetErrorMessage() {return mErrorMessage.c_str();}

private:
    bool Error(const wchar_t *msg);

    // Any current error message
    std::wstring mErrorMessage;

    // The root node
    CComPtr<IXMLDOMNode> mRootnode;

    // The document
    CComPtr<IXMLDOMDocument>  mXMLDoc;

    // Stack of elements
    struct Element 
    {
        Element(IXMLDOMElement *element, IXMLDOMNode *node) {mElement = element; mNode = node;}
        CComPtr<IXMLDOMElement> mElement;
        CComPtr<IXMLDOMNode> mNode;
    };

    std::list<Element> mElementsStack;
};

//! \endcond
