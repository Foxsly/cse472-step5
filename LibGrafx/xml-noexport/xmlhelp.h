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
//! \cond ignore

#include <string>
#include <sstream>


//
// Name :         xmlhelp.h
// Description :  Routines that simplify coding for XML reading and writing in this application.
//

//
// Name :  NextNode()
// Description : This function accepts a reference to a node pointer
//               and advances it to the next sibling node. 
//

inline void NextNode(CComPtr<IXMLDOMNode> &node)
{
    CComPtr<IXMLDOMNode> next;
    node->get_nextSibling(&next);
    node = next;
}

// Some inline functions I like to use with XML code to make things faster and easier:
inline void Get(CComPtr<IXMLDOMNodeList> &p_nodes, int i, CComPtr<IXMLDOMNode> &node, DOMNodeType &nodeType, CComBSTR &nodeName)
{
    p_nodes->get_item(i, &node);
    node->get_nodeType(&nodeType);
    node->get_nodeName(&nodeName);
}

inline void Get(CComPtr<IXMLDOMNodeList> &p_nodes, int i, CComPtr<IXMLDOMNode> &node, DOMNodeType &nodeType, CComBSTR &nodeName, CComVariant &nodeValue)
{
    p_nodes->get_item(i, &node);
    node->get_nodeType(&nodeType);
    node->get_nodeName(&nodeName);
    node->get_nodeValue(&nodeValue);
}

inline void Get(CComPtr<IXMLDOMNamedNodeMap> &p_nodes, int i, CComPtr<IXMLDOMNode> &node, DOMNodeType &nodeType, CComBSTR &nodeName, CComVariant &nodeValue)
{
    p_nodes->get_item(i, &node);
    node->get_nodeType(&nodeType);
    node->get_nodeName(&nodeName);
    node->get_nodeValue(&nodeValue);
}


//
// Name :         ObtainNodeText()
// Description :  Given a node we don't expect to have any children, this
//                function obtains the node TEXT field and returns it in 
//                p_value as a VARIANT.
//

inline bool ObtainNodeText(IXMLDOMNode *p_node, CComVariant &p_value)
{
	//
    // Traverse any children
    //

    CComPtr<IXMLDOMNodeList> childNodes;
    p_node->get_childNodes(&childNodes);

	long len;
    childNodes->get_length(&len);

    for(int i=0;  i<len;  i++)
    {
        // So, what all do we have under us?
        CComPtr<IXMLDOMNode>    node;
        DOMNodeType nodeType;
        CComVariant nodeValue;

		childNodes->get_item(i, &node);
		node->get_nodeType(&nodeType);
		node->get_nodeValue(&p_value);

        if(nodeType == NODE_TEXT)
			return true;
	}

	p_value.Clear();
	return false;
}

////////////////////////////////////////////////////////////////////////////////////
//
// Setting Attributes
//
////////////////////////////////////////////////////////////////////////////////////

inline void SetXmlAttribute(IXMLDOMElement *element, BSTR name, const std::wstring &value)
{
    element->setAttribute(name, CComVariant(value.c_str()));
}

inline void SetXmlAttribute(IXMLDOMElement *element, BSTR name, const wchar_t *value)
{
    element->setAttribute(name, CComVariant(value));
}

inline void SetXmlAttribute(IXMLDOMElement *element, BSTR name, float value)
{
    element->setAttribute(name, CComVariant(value));
}

inline void SetXmlAttribute(IXMLDOMElement *element, BSTR name, double value)
{
    element->setAttribute(name, CComVariant(value));
}

inline void SetXmlAttribute(IXMLDOMElement *element, BSTR name, int value)
{
    element->setAttribute(name, CComVariant(value));
}


inline void SetXmlAttribute3fv(IXMLDOMElement *element, BSTR name, const float *value)
{
    std::wstringstream str;
    str << value[0] << " " << value[1] << " " << value[2] << std::ends;
    element->setAttribute(name, CComVariant(str.str().c_str()));
}


////////////////////////////////////////////////////////////////////////////////////
//
// Getting Attributes
//
////////////////////////////////////////////////////////////////////////////////////


// This template class allows us to get an attribute form a node rather 
// than having to map to attributes first.
template <class T> bool GetXmlAttribute(IXMLDOMNode *node, BSTR name, T &value)
{
    CComPtr<IXMLDOMNamedNodeMap> attributes;
    node->get_attributes(&attributes);
    return GetXmlAttribute(attributes, name, value);
}


// Get an attribute from a named node map
inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, BSTR name, std::wstring &value)
{
    CComPtr<IXMLDOMNode> node;
    attributes->getNamedItem(name, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    value = valueV.bstrVal;
    return true;
}


inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, BSTR name, bool &value)
{
    CComPtr<IXMLDOMNode> node;
    attributes->getNamedItem(name, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    std::wstring v(valueV.bstrVal);
    value = v == L"true";
    return true;
}


inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, BSTR name, int &value)
{
    CComPtr<IXMLDOMNode> node;
    attributes->getNamedItem(name, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    valueV.ChangeType(VT_I4);
    value = valueV.lVal;
    return true;
}

inline bool GetXmlAttribute(IXMLDOMNamedNodeMap *attributes, BSTR name, double &value)
{
    CComPtr<IXMLDOMNode> node;
    attributes->getNamedItem(name, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    valueV.ChangeType(VT_R8);   // Indicates Real 8 byte value
    value = valueV.dblVal;      // VT_R8 puts the value here
    return true;
}



inline bool GetXmlAttribute3fv(IXMLDOMNamedNodeMap *attributes, BSTR name, float *value)
{
    CComPtr<IXMLDOMNode> node;
    attributes->getNamedItem(name, &node);
    if(node == NULL)
        return false;

    CComVariant valueV;
    node->get_nodeValue(&valueV);
    const wchar_t *p = valueV.bstrVal;
    for(int j=0;  j<3;  j++)
    {
        value[j] = (float)_wtof(p);

        // Skip over the number
        while(*p && *p != ' ') p++;

        // Skip over any whitespace
        while(*p == ' ') p++;
    }
    return true;
}


inline CComPtr<IXMLDOMNode> GetXmlChildNode(IXMLDOMNode *topNode, BSTR name)
{
    // Find the child connections tag
    CComPtr<IXMLDOMNode> node;
    topNode->get_firstChild(&node);
    for( ;  node != NULL;  NextNode(node))
    {
        DOMNodeType nodeType;
        CComBSTR nodeName;

        node->get_nodeType(&nodeType);
        node->get_nodeName(&nodeName);

        if(nodeType == NODE_ELEMENT)
        {
            if(nodeName == "connection")
                return node;
        }
    }

    return NULL;
}

//! \endcond


