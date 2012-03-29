#pragma once
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

/*!
\mainpage LibGrafx graphics tools library.

\author Charles B. Owen

\version 02-19-2012 1.01 Library version declared.

\section Introduction

This library provides basic graphics tools including vectors, matrices, texture images,
simple images, and a graphics file format ModelX loader that can display models
output by the XNA Model Viewer available from http://modelviewer.codeplex.com/.

To use, #include "grafx.h" only. Do not directly #include other headers from
the library.

\section License

This work is Copyright 2002-2012 Michigan State University
This work is licensed under Microsoft Public License (Ms-PL)

Please include author attribution when using this code.
*/

#if !defined(LibGrafx)
#define LibGrafx __declspec(dllimport)
#pragma comment(lib, "LibGrafx")
#endif

#include "graphics-noexport/GrVector.h"
#include "graphics-noexport/GrTransform.h"
#include "graphics-noexport/GrTexture.h"
#include "graphics-noexport/GrSphere.h"
#include "graphics-noexport/GrModelX.h"
#include "graphics-noexport/GrImage.h"


