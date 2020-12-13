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
#pragma once

#include "Base/DynamicBufferRing.h"
#include "Base/Texture.h"

#define SPD_MAX_MIP_LEVELS 12



namespace CAULDRON_DX12
{
    enum class SPDWaveOps
    {
        SPDNoWaveOps,
        SPDWaveOps,
    };

    enum class SPDPacked
    {
        SPDNonPacked,
        SPDPacked,
    };

    enum class SPDLoad
    {
        SPDLoad,
        SPDLinearSampler,
    };

    class SPDCS
    {
    public:
        void OnCreate(Device *pDevice, ResourceViewHeaps *pResourceViewHeaps, DynamicBufferRing *pConstantBufferRing, 
            SPDLoad spdLoad, SPDWaveOps spdWaveOps, SPDPacked spdPacked);
        void OnDestroy();

        void Draw(ID3D12GraphicsCommandList2 *pCommandList);

        void CreateDisplayResources(Texture *t);

        void SetTexture(Texture *t, bool display)
        { 
           if (display && (t != m_texture))
            {
               CreateDisplayResources(t);
            }

            m_texture = t; 
        }


        Texture *GetTexture() { return m_texture; }

        void GUI(int *pSlice);

        struct SpdConstants
        {
            int mips;
            int numWorkGroupsPerSlice;
            int workGroupOffset[2];
        };

        struct SpdLinearSamplerConstants
        {
            int mips;
            int numWorkGroupsPerSlice;
            int workGroupOffset[2];
            float invInputSize[2];
            float padding[2];
        };


        Device                       *m_pDevice = nullptr;
    private:

        Texture                       *m_texture;

        CBV_SRV_UAV                   m_constBuffer; // dimension
        CBV_SRV_UAV                   m_SRV[SPD_MAX_MIP_LEVELS * 6]; // for display of mips using imGUI
        CBV_SRV_UAV                   m_globalCounter;
        Texture                       m_globalCounterBuffer;

        ResourceViewHeaps            *m_pResourceViewHeaps = nullptr;
        DynamicBufferRing            *m_pConstantBufferRing = nullptr;
        ID3D12RootSignature          *m_pRootSignature = nullptr;
        ID3D12PipelineState          *m_pPipeline = nullptr;

        SPDLoad                       m_spdLoad;
        SPDWaveOps                    m_spdWaveOps;
        SPDPacked                     m_spdPacked;
    };
}


    // Caller must persist this until the mipmap command list is known to be processed, which they may do by calling CommandQueue->Signal after associated ExecuteCommandList.
   class Mipmap_View
   {
   public:
      void Mipmap_View::Init(Texture *tex, ResourceViewHeaps *ResourceViewHeaps, SPDLoad spdLoad);

      // Mipmap processing requirement
      CAULDRON_DX12::CBV_SRV_UAV                   m_UAV[SPD_MAX_MIP_LEVELS + 1]; //src + dest mips
      CAULDRON_DX12::CBV_SRV_UAV                   m_sourceSRV;
   } ;