//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#include "StdAfx.h"
#include "XmlDocument.h"

#include <sstream>
#include "xmlhelp.h"


using namespace std;

CXmlDocument::CXmlDocument(void)
{
    ::CoInitialize(NULL);
}

CXmlDocument::~CXmlDocument(void)
{
    // Ensure all is destroyed before we do the uninitialize
    mElementsStack.clear();
    mRootnode.Release();
    mXMLDoc.Release();
    ::CoUninitialize();
}


//
// Name :         CXmlDocument::Error()
// Description :  Error indication function. Returns false and sets an error message.
//
bool CXmlDocument::Error(const wchar_t *msg)
{
    mErrorMessage = msg;
    return false;
}


bool CXmlDocument::CreateXmlDocument(const wchar_t *rootName)
{
    //
    // Create an XML document
    //

	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,  IID_IXMLDOMDocument, (void**)&mXMLDoc));
	if(!succeeded)
		return Error(L"Unable to create XML document");

    mXMLDoc->put_preserveWhiteSpace(VARIANT_TRUE);

    CComPtr<IXMLDOMNode> node;

    // Create a processing instruction targeted for xml.
    CComPtr<IXMLDOMProcessingInstruction> pi;
    mXMLDoc->createProcessingInstruction(L"xml", L"version='1.0'", &pi);
    mXMLDoc->appendChild(pi, &node);
    pi.Release();
    node.Release();

    mXMLDoc->createProcessingInstruction(L"xml-stylesheet", L"type='text/xml' href='dom.xsl'", &pi);
    mXMLDoc->appendChild(pi, &node);
    pi.Release();
    node.Release();

    // Create the root element
    CComPtr<IXMLDOMElement> pe;
    mXMLDoc->createElement(CComBSTR(rootName), &pe);
    mXMLDoc->appendChild(pe, &mRootnode);
    pe.Release();

    // Push the root element onto the stack
    mElementsStack.push_front(Element(pe, mRootnode));

    return true;
}

void CXmlDocument::PushElement(const wchar_t *elementName)
{
    // Create  a new element
    CComPtr<IXMLDOMNode> node;
    CComPtr<IXMLDOMElement> element;
    mXMLDoc->createElement(CComBSTR(elementName), &element);
    GetCurrentNode()->appendChild(element, &node);

    // Push the root element onto the stack
    mElementsStack.push_front(Element(element, node));
}


void CXmlDocument::PopElement()
{
    mElementsStack.pop_front();
}


void CXmlDocument::SetAttribute(const wchar_t *name, const wchar_t *value)
{
    GetCurrentElement()->setAttribute(CComBSTR(name), CComVariant(value));
}


void CXmlDocument::SetAttribute(const wchar_t *name, int value)
{
    GetCurrentElement()->setAttribute(CComBSTR(name), CComVariant(value));
}

void CXmlDocument::SetAttribute(const wchar_t *name, double value)
{
    GetCurrentElement()->setAttribute(CComBSTR(name), CComVariant(value));
}



void CXmlDocument::SetAttribute(const wchar_t *name, bool value)
{
    GetCurrentElement()->setAttribute(CComBSTR(name), CComVariant(value ? L"true" : L"false"));
}





bool CXmlDocument::WriteXmlDocument(const wchar_t *fileName)
{
    //
    // The following code uses an XSLT style sheet to pretty-format the XML result
    //

    // Create an indenter document
    CComPtr<IXMLDOMDocument> indenterDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,  IID_IXMLDOMDocument, (void**)&indenterDoc));
	if(!succeeded)
		return Error(L"Unable to create XML indenter document");

    VARIANT_BOOL success;
    indenterDoc->loadXML(L"<?xml version=\"1.0\"?>"
L"<xsl:stylesheet version=\"1.0\" xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
L"  <xsl:output method=\"xml\" encoding=\"ISO-8859-1\"/>"
L"  <xsl:param name=\"indent-increment\" select=\"'   '\"/>"
L"  "
L"  <xsl:template name=\"newline\">"
L"    <xsl:text disable-output-escaping=\"yes\">"
L"</xsl:text>"
L"  </xsl:template>"
L"  "
L"  <xsl:template match=\"comment() | processing-instruction()\">"
L"    <xsl:param name=\"indent\" select=\"''\"/>"
L"    <xsl:call-template name=\"newline\"/>    "
L"    <xsl:value-of select=\"$indent\"/>"
L"    <xsl:copy />"
L"  </xsl:template>"
L" "
L"  <xsl:template match=\"text()\">"
L"    <xsl:param name=\"indent\" select=\"''\"/>"
L"    <xsl:call-template name=\"newline\"/>    "
L"    <xsl:value-of select=\"$indent\"/>"
L"    <xsl:value-of select=\"normalize-space(.)\"/>"
L"  </xsl:template>"
L"    "
L"  <xsl:template match=\"text()[normalize-space(.)='']\"/>"
L"  "
L"  <xsl:template match=\"*\">"
L"    <xsl:param name=\"indent\" select=\"''\"/>"
L"    <xsl:call-template name=\"newline\"/>    "
L"    <xsl:value-of select=\"$indent\"/>"
L"      <xsl:choose>"
L"       <xsl:when test=\"count(child::*) > 0\">"
L"        <xsl:copy>"
L"         <xsl:copy-of select=\"@*\"/>"
L"         <xsl:apply-templates select=\"*|text()\">"
L"           <xsl:with-param name=\"indent\" select=\"concat ($indent, $indent-increment)\"/>"
L"         </xsl:apply-templates>"
L"         <xsl:call-template name=\"newline\"/>"
L"         <xsl:value-of select=\"$indent\"/>"
L"        </xsl:copy>"
L"       </xsl:when>       "
L"       <xsl:otherwise>"
L"        <xsl:copy-of select=\".\"/>"
L"       </xsl:otherwise>"
L"     </xsl:choose>"
L"  </xsl:template>    "
L"</xsl:stylesheet>", &success);

    CComPtr<IXMLDOMDocument> indentedDoc;
	succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,  IID_IXMLDOMDocument, (void**)&indentedDoc));
	if(!succeeded)
		return Error(L"Unable to create XML intented document");

    BSTR xmlStr;
    mXMLDoc->transformNode(indenterDoc, &xmlStr);

    indentedDoc->loadXML(xmlStr, &success);
    indentedDoc->save(CComVariant(fileName));

    mRootnode.Release();
    mXMLDoc.Release();
    mElementsStack.clear();

    return true;
}



bool CXmlDocument::OpenXmlDocument(const wchar_t *fileName, const wchar_t *rootName)
{
    //
    // Create an XML document
    //

    CComPtr<IXMLDOMDocument>  mXMLDoc;
	bool succeeded = SUCCEEDED(CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER,  IID_IXMLDOMDocument, (void**)&mXMLDoc));
	if(!succeeded)
		return false;

	// Open the XML document
    VARIANT_BOOL ok;
    succeeded = SUCCEEDED(mXMLDoc->load(CComVariant(fileName), &ok));
	if(!succeeded || ok == VARIANT_FALSE)
		return Error((wstring(L"Unable to open ") + fileName).c_str());

    // Find the root node
    CComPtr<IXMLDOMNodeList> nodes;
    mXMLDoc->get_childNodes(&nodes);		// Obtain a list of child nodes...

    long len;
    nodes->get_length(&len);                // How many items in the node?
    for(int i=0;  i<len;  i++)
    {
        CComPtr<IXMLDOMNode>    node;
        DOMNodeType nodeType;
        CComBSTR nodeName;

        Get(nodes, i, node, nodeType, nodeName);
        if(nodeType == NODE_ELEMENT && nodeName == rootName)
        {
            mRootnode = node;
            return true;
        }
    }

    return Error((wstring(L"XML file does not contain root tag ") + rootName).c_str());;
}

