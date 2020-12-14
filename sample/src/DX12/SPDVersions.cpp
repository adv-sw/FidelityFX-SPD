// SPDSample
//
// Copyright (c) 2020 Advanced Micro Devices, Inc. All rights reserved.
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

#include "stdafx.h"
#include "base/DynamicBufferRing.h"
#include "base/StaticBufferPool.h"
#include "base/UploadHeap.h"
#include "base/Texture.h"
#include "base/Helper.h"
#include "SPDVersions.h"

namespace CAULDRON_DX12
{
    void SPDVersions::OnCreate(
        Device *pDevice,
        UploadHeap *pUploadHeap,
        ResourceViewHeaps *pResourceViewHeaps,
        DynamicBufferRing *pConstantBufferRing
    )
    {
        m_pDevice = pDevice;
        m_pResourceViewHeaps = pResourceViewHeaps;
        m_pConstantBufferRing = pConstantBufferRing;
    }

    uint32_t SPDVersions::GetMaxMIPLevelCount(uint32_t Width, uint32_t Height)
    {
        uint32_t resolution = max(Width, Height);
        return (static_cast<uint32_t>(min(floor(log2(resolution)), 12)));
    }

    void SPDVersions::OnDestroy()
    {
        m_spd_WaveOps_NonPacked.OnDestroy();
        m_spd_WaveOps_Packed.OnDestroy();
        m_spd_No_WaveOps_NonPacked.OnDestroy();
        m_spd_No_WaveOps_Packed.OnDestroy();

        m_spd_WaveOps_NonPacked_Linear_Sampler.OnDestroy();
        m_spd_WaveOps_Packed_Linear_Sampler.OnDestroy();
        m_spd_No_WaveOps_NonPacked_Linear_Sampler.OnDestroy();
        m_spd_No_WaveOps_Packed_Linear_Sampler.OnDestroy();
    }

    
            
    void SPDVersions::Dispatch(Texture *pTarget, ID3D12GraphicsCommandList2 *pCommandList, SPDLoad spdLoad, SPDWaveOps spdWaveOps, SPDPacked spdPacked, bool app_mode)
    {
       switch (spdLoad)
        {
        case SPDLoad::SPDLoad:
        {
            switch (spdWaveOps)
            {
            case SPDWaveOps::SPDWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                   if (!m_spd_WaveOps_NonPacked.m_pDevice)
                      m_spd_WaveOps_NonPacked.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLoad, SPDWaveOps::SPDWaveOps, SPDPacked::SPDNonPacked);

                    m_spd_WaveOps_NonPacked.SetTexture(pTarget, app_mode);
                    m_spd_WaveOps_NonPacked.Draw(pCommandList);
                    break;

                case SPDPacked::SPDPacked:
                   if (!m_spd_WaveOps_Packed.m_pDevice)
                      m_spd_WaveOps_Packed.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLoad, SPDWaveOps::SPDWaveOps, SPDPacked::SPDPacked);
 
                    m_spd_WaveOps_Packed.SetTexture(pTarget, app_mode);
                    m_spd_WaveOps_Packed.Draw(pCommandList);
                    break;
                }
                break;

            case SPDWaveOps::SPDNoWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                   if (!m_spd_No_WaveOps_NonPacked.m_pDevice)
                     m_spd_No_WaveOps_NonPacked.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLoad, SPDWaveOps::SPDNoWaveOps, SPDPacked::SPDNonPacked);
                    
                    m_spd_No_WaveOps_NonPacked.SetTexture(pTarget, app_mode);
                    m_spd_No_WaveOps_NonPacked.Draw(pCommandList);
                    break;

                case SPDPacked::SPDPacked:
                    if (!m_spd_No_WaveOps_Packed.m_pDevice) 
                       m_spd_No_WaveOps_Packed.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing,SPDLoad::SPDLoad, SPDWaveOps::SPDNoWaveOps, SPDPacked::SPDPacked);
                    
                    m_spd_No_WaveOps_Packed.SetTexture(pTarget, app_mode);
                    m_spd_No_WaveOps_Packed.Draw(pCommandList);
                    break;
                }
            }
            break;
        }
        case SPDLoad::SPDLinearSampler:
        {
            switch (spdWaveOps)
            {
            case SPDWaveOps::SPDWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                   if (!m_spd_WaveOps_NonPacked_Linear_Sampler.m_pDevice) 
                       m_spd_WaveOps_NonPacked_Linear_Sampler.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLinearSampler, SPDWaveOps::SPDWaveOps, SPDPacked::SPDNonPacked);

                    m_spd_WaveOps_NonPacked_Linear_Sampler.SetTexture(pTarget, app_mode); 
                    m_spd_WaveOps_NonPacked_Linear_Sampler.Draw(pCommandList);
                    break;

                
                case SPDPacked::SPDPacked:
                   if (!m_spd_WaveOps_Packed_Linear_Sampler.m_pDevice) 
                       m_spd_WaveOps_Packed_Linear_Sampler.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLinearSampler, SPDWaveOps::SPDWaveOps, SPDPacked::SPDPacked);
                    
                    m_spd_WaveOps_Packed_Linear_Sampler.SetTexture(pTarget, app_mode); 
                    m_spd_WaveOps_Packed_Linear_Sampler.Draw(pCommandList);
                    break;
                }
                break;

            case SPDWaveOps::SPDNoWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                   if (!m_spd_No_WaveOps_NonPacked_Linear_Sampler.m_pDevice)       
                       m_spd_No_WaveOps_NonPacked_Linear_Sampler.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLinearSampler, SPDWaveOps::SPDNoWaveOps, SPDPacked::SPDNonPacked);
 
                    m_spd_No_WaveOps_NonPacked_Linear_Sampler.SetTexture(pTarget, app_mode); 
                    m_spd_No_WaveOps_NonPacked_Linear_Sampler.Draw(pCommandList);
                    break;

                case SPDPacked::SPDPacked:
                           
                    if (!m_spd_No_WaveOps_Packed_Linear_Sampler.m_pDevice) 
                       m_spd_No_WaveOps_Packed_Linear_Sampler.OnCreate(m_pDevice, m_pResourceViewHeaps, m_pConstantBufferRing, SPDLoad::SPDLinearSampler, SPDWaveOps::SPDNoWaveOps, SPDPacked::SPDPacked);

                    m_spd_No_WaveOps_Packed_Linear_Sampler.SetTexture(pTarget, app_mode); 
                    m_spd_No_WaveOps_Packed_Linear_Sampler.Draw(pCommandList);
                    break;
                }
            }
            break;
        }
        }
    }

    void SPDVersions::GUI(SPDLoad spdLoad, SPDWaveOps spdWaveOps, SPDPacked spdPacked, int *pSlice)
    {
        switch (spdLoad)
        {
        case SPDLoad::SPDLoad:
        {
            switch (spdWaveOps)
            {
            case SPDWaveOps::SPDWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                    m_spd_WaveOps_NonPacked.GUI(pSlice);
                    break;
                case SPDPacked::SPDPacked:
                    m_spd_WaveOps_Packed.GUI(pSlice);
                    break;
                }
                break;
            case SPDWaveOps::SPDNoWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                    m_spd_No_WaveOps_NonPacked.GUI(pSlice);
                    break;
                case SPDPacked::SPDPacked:
                    m_spd_No_WaveOps_Packed.GUI(pSlice);
                    break;
                }
            }
            break;
        }
        case SPDLoad::SPDLinearSampler:
        {
            switch (spdWaveOps)
            {
            case SPDWaveOps::SPDWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                    m_spd_WaveOps_NonPacked_Linear_Sampler.GUI(pSlice);
                    break;
                case SPDPacked::SPDPacked:
                    m_spd_WaveOps_Packed_Linear_Sampler.GUI(pSlice);
                    break;
                }
                break;
            case SPDWaveOps::SPDNoWaveOps:
                switch (spdPacked)
                {
                case SPDPacked::SPDNonPacked:
                    m_spd_No_WaveOps_NonPacked_Linear_Sampler.GUI(pSlice);
                    break;
                case SPDPacked::SPDPacked:
                    m_spd_No_WaveOps_Packed_Linear_Sampler.GUI(pSlice);
                    break;
                }
            }
            break;
        }
        }
    }
}




// DX12 extension API implemented here for now - TODO: Move into a seperate implementation file.

class Extension_Mipmaps 
{
public:
   SPDVersions      *_mipmap_engine = nullptr;
   ResourceViewHeaps _resourceViewHeaps;
   CommandListRing   _commandListRing;
   DynamicBufferRing _constantBufferRing;
   ID3D12Fence      *_processing_fence = nullptr;
   UINT64            _current_fence_value = 1;
   Texture          _tex;
   Device _device;
};

static Extension_Mipmaps __api;


#define DX12_EXTENSION_API_EXPORTS
#include "..\..\..\lib\dx12_ext_mipmap.h"
#include "..\..\libs\cauldron\src\common\base\ShaderCompilerCache.h"
#include "..\..\libs\cauldron\src\common\Misc\DxgiFormatHelper.h"
#include "shlobj_core.h"
bool InitDirectXCompiler();



void Mipmap_View::Init(Texture *tex, ResourceViewHeaps *ResourceViewHeaps, SPDLoad spdLoad)
{
      if (spdLoad == SPDLoad::SPDLinearSampler)
      {
         ResourceViewHeaps->AllocCBV_SRV_UAVDescriptor(1, &m_sourceSRV);
         tex->CreateSRV(0, &m_sourceSRV, 0, tex->GetArraySize(), 0);
      }

      uint32_t numUAVs = tex->GetMipCount();
        
      if (spdLoad == SPDLoad::SPDLinearSampler)
      {
         // we need one UAV less because source texture will be bound as SRV and not as UAV
         numUAVs = tex->GetMipCount() - 1;
      }

      ResourceViewHeaps->AllocCBV_SRV_UAVDescriptor(numUAVs, m_UAV);

      // Create views for the mip chain
      //
      // destination 
      //
      for (uint32_t i = 0; i < numUAVs; i++)
      {
         D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
         uavDesc.Format = ConvertIntoNonGammaFormat(tex->GetFormat());   // can't create SRGB UAV
         uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
         uavDesc.Texture2DArray.ArraySize = tex->GetArraySize();
         uavDesc.Texture2DArray.FirstArraySlice = 0;
            
         if (spdLoad == SPDLoad::SPDLinearSampler)
         {
               uavDesc.Texture2DArray.MipSlice = i + 1;
         }
         else 
         {
               uavDesc.Texture2DArray.MipSlice = i;
         }

         uavDesc.Texture2DArray.PlaneSlice = 0;

         tex->CreateUAV(i, NULL, m_UAV, &uavDesc);
      }
}


Mipmap_View *Ext_Mipmaps_Create(Texture *t,  ResourceViewHeaps *rh, SPDLoad spd_load)
{
   Mipmap_View *mipview = new Mipmap_View;
   mipview->Init(t, rh, spd_load);
   return mipview;
}



// Could be COM ref counted but isn't right now.
DX12_EXTENSION_API void Ext_Mipmaps_Delete(Mipmap_View *mipview)
{
   // TODO: Free up whatever space we can on heaps now we know we no longer need our resources.
   
   delete mipview;
}


DX12_EXTENSION_API Mipmap_View *Ext_Mipmaps_Generate(ID3D12GraphicsCommandList2 *cmd_list, ID3D12Resource *target, Mipmap_View *current)
{
   if (!__api._mipmap_engine)
   {
       // TODO: Implement Ext_GenerateMipmaps_Configure to specify these paths externally.
       char app_path[MAX_PATH+50];
       GetModuleFileNameA(nullptr, app_path, MAX_PATH);
       char *ptr = strrchr(app_path, '\\');
       
       if (ptr)
          *ptr = '\0';

       strcat(app_path, "\\ext_dx12");

       char cache_path[MAX_PATH];
       SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL,  0, cache_path);

       InitShaderCompilerCache(app_path, cache_path);
       bool ok = InitDirectXCompiler();
      
       ID3D12Device *dev;
       cmd_list->GetDevice( __uuidof(ID3D12Device), (void**) &dev);
       __api._mipmap_engine = new SPDVersions;
       __api._device.SetDevice(dev);
       
       // Initialize helpers

       // Create all the heaps for the resources views
       const uint32_t cbvDescriptorCount = 2000;
       const uint32_t srvDescriptorCount = 2000;
       const uint32_t uavDescriptorCount = 10;
       const uint32_t dsvDescriptorCount = 10;
       const uint32_t rtvDescriptorCount = 60;
       const uint32_t samplerDescriptorCount = 20;
       __api._resourceViewHeaps.OnCreate(&__api._device, cbvDescriptorCount, srvDescriptorCount, uavDescriptorCount, dsvDescriptorCount, rtvDescriptorCount, samplerDescriptorCount);

       const uint32_t backBufferCount = 3;

       // Create a 'dynamic' constant buffer
       const uint32_t constantBuffersMemSize = 20 * 1024 * 1024;    
       __api._constantBufferRing.OnCreate(&__api._device, backBufferCount, constantBuffersMemSize, &__api._resourceViewHeaps);

       __api._mipmap_engine->OnCreate(&__api._device, nullptr, &__api._resourceViewHeaps, &__api._constantBufferRing);
   }

   D3D12_RESOURCE_DESC rdesc = target->GetDesc();
   __api._tex.m_header.width = rdesc.Width;
   __api._tex.m_header.height = rdesc.Height;
   __api._tex.m_header.mipMapCount = rdesc.MipLevels;
   __api._tex.m_header.arraySize = rdesc.DepthOrArraySize;
   __api._tex.m_header.format = rdesc.Format;

   Mipmap_View *mipview = current ? current : new Mipmap_View;

   __api._tex.m_mipview = mipview;
   __api._tex.SetResource(target);

   SPDLoad spd_load = SPDLoad::SPDLoad;

   if (!current)
      mipview->Init(&__api._tex, &__api._resourceViewHeaps, spd_load);

   __api._mipmap_engine->Dispatch(&__api._tex, cmd_list, spd_load, SPDWaveOps::SPDWaveOps, SPDPacked::SPDPacked, false);

   return mipview; // All good.
}


DX12_EXTENSION_API void Ext_Mipmaps_Terminate()
{
   __api._resourceViewHeaps.OnDestroy();
   __api._commandListRing.OnDestroy();
   __api._constantBufferRing.OnDestroy();

   __api._mipmap_engine->OnDestroy();
   delete __api._mipmap_engine;
   __api._mipmap_engine = nullptr;
}
