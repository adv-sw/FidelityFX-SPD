// SPDSample
//
// Copyright (c) 2020 Advance Software Limited. All rights reserved.
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
 
#include <d3d12.h> 

#ifdef DX12_EXTENSION_API_EXPORTS
#define DX12_EXTENSION_API  extern "C" __declspec(dllexport)
#else
#define DX12_EXTENSION_API  extern "C" __declspec(dllimport)
#endif // DX12_EXTENSION_API_EXPORTS


class Mipmap_View;

DX12_EXTENSION_API void Ext_Mipmaps_Delete(Mipmap_View *mipview);
DX12_EXTENSION_API Mipmap_View *Ext_Mipmaps_Generate(ID3D12GraphicsCommandList2 *cl, ID3D12Resource *target,  Mipmap_View *current = nullptr);
DX12_EXTENSION_API void Ext_Mipmaps_Terminate();
