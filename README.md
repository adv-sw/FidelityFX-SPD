# DirectX12 Extension: Mipmap Generation.
Copyright (c) 2020 Advance Software Limited. All rights reserved. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


Implements mipmap extension using a modified AMD FidelityFX SPD.

Interface as follows, defined in lib/dx12_ext_mipmap.h

DX12_EXTENSION_API Mipmap_View *Ext_Mipmaps_Generate(ID3D12GraphicsCommandList2 *cl, ID3D12Resource *target,  Mipmap_View *current = nullptr);

Generate mipmaps for specified resource (2d texture or cubemap), returning a view required to complete processing. Persist the view and pass in next run for dynamic textures or delete it using Ext_Mipmaps_Delete when you've executed the command list mip generation has been defined in.

DX12_EXTENSION_API void Ext_Mipmaps_Delete(Mipmap_View *mipview);

Delete a mipmap view when no longer required. For example, once the command list that created the mipmaps for your resource has executed.

DX12_EXTENSION_API void Ext_Mipmaps_Terminate();

Terminate & clean up mip map extension resources. This can be done at application exit for a clean shutdown.


Usage : Ensure your texture resources are created with the required mip chain and have D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS specified to allow mip generation to proceed.



# Build Instructions

1. Build as per AMD FidelityFX SPD instructions below - this should result in an interactive test app.
2. Modify project settings from app to DLL & create your DLL & import library wherever you want them.
3. Link import library into your app & call API functions to generate mips for your textures.
4. Place shader assets in your app root directory or precompile & link shader some other way - you have the source.




Future work :

1. Implement Vulkan equivalent.
2. Merge into AMD trunk implementation.
3. Optimizations.
4. Pass in configuration options to support a wider range of uses.
5. Update cmake files to support library mode.
6. Rework to remove requirement to perform minor modifications to Cauldron or commit those changes to Cauldron.




# FidelityFX SPD
Copyright (c) 2020 Advanced Micro Devices, Inc. All rights reserved. Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Changelist v2.0

- Added support for cube and array textures. SpdDownsample and SpdDownsampleH shader functions now take index of texture slice as an additional parameter. For regular texture use 0.
- Added support for updating only sub-rectangle of the texture. Additional, optional parameter workGroupOffset added to shader functions SpdDownsample and SpdDownsampleH.
- Added C function SpdSetup that helps to setup constants to be passed as a constant buffer.
- The global atomic counter is automatically reset to 0 by the shader at the end, so you do not need to clear it before every use, just once after creation

# Single Pass Downsampler - SPD

FidelityFX Single Pass Downsampler (SPD) provides an RDNA-optimized solution for generating up to 12 MIP levels of a texture.
- Generates up to 12 MIP levels (maximum source texture size is 4096x4096) per slice.
- Supports Texture2DArrays / CubeTextures: downsamples all slices within one single disptach call.
- Single compute dispatch.
- User defined 2x2 reduction function.
- User controlled border handling.
- Supports various image formats.
- HLSL and GLSL versions available.
- Rapid Packed Math support.
- Uses optionally subgroup operations / SM6+ wave operations, which can provide faster performance.
- Supports downsampling of a sub-rectangle from the source texture: useful for atlas textures in which only a known region got updated

# Build Instructions

1. Clone submodules by running 'git submodule update --init --recursive' (so you get the Cauldron framework too)
2. cd [project]/sample/build
3. run GenerateSolutions.bat
3. Open solution, build + run + enjoy

# SPD Files
You can find them in ffx-spd
- ffx_a.h: helper file
- ffx_spd: contains the SPD function and integration documentation

# Sample
Downsampler
- PS: computes each mip in a separate pixel shader pass
- Multipass CS: computes each mip in a separate compute shader pass
- SPD CS: uses the SPD library, computes all mips (up to a source texture of size 4096²) in a single pass

SPD Load Versions
- Load: uses a load to fetch from the source texture
- Linear Sampler: uses a sampler to fetch from the source texture. Sampler must meet the user defined reduction function.

SPD WaveOps Versions
- No-WaveOps: uses only LDS to share the data between threads
- WaveOps: uses Intrinsics and LDS to share the data between threads

SPD Non-Packed / Packed Versions
- Non-Packed: uses fp32
- Packed: uses fp16, reduced register pressure

# Recommendations
We recommend to use the WaveOps path when supported. If higher precision is not needed, you can enable the packed mode - it has less register pressure and can run a bit faster as well.
If you compute the average for each 2x2 quad, we also recommend to use a linear sampler to fetch from the source texture instead of four separate loads.

# Known issues
Please use driver 20.8.3 or newer. There is a known issue on DX12 when using the SPD No-WaveOps Packed version.
It may appear as "Access violation reading location ..." during CreateComputePipelineState, with top of the stack
pointing to amdxc64.dll.
To workaround this issue, you may advise players to update their graphics driver or don't compile and use
a different SPD version, e.g. a Non-Packed version.