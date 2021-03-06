/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __itkVanHerkGilWermanUtilities_h
#define __itkVanHerkGilWermanUtilities_h

#include <list>

#include "itkSharedMorphologyUtilities.h"

namespace itk
{
/**
 * \class VanHerkGilWermanUtilities
 * \brief functionality in common for anchor openings/closings and
 * erosions/dilation
 *
 */
#if defined( _MSC_VER ) && _MSC_VER >= 1300

template< class PixelType, class TFunction >
void FillReverseExt(std::vector<PixelType> & pixbuffer,
                    std::vector<PixelType> & rExtBuffer,
                    const unsigned int KernLen, unsigned len);

template< class PixelType, class TFunction >
void FillForwardExt(std::vector<PixelType> & pixbuffer,
                    std::vector<PixelType> & fExtBuffer,
                    const unsigned int KernLen, unsigned len);

template< class TImage, class TBres, class TFunction, class TLine >
void DoFace(typename TImage::ConstPointer input,
            typename TImage::Pointer output,
            typename TImage::PixelType border,
            TLine line,
            const typename TBres::OffsetArray LineOffsets,
            const unsigned int KernLen,
            std::vector<typename TImage::PixelType> & pixbuffer,
            std::vector<typename TImage::PixelType> & fExtBuffer,
            std::vector<typename TImage::PixelType> & rExtBuffer,
            const typename TImage::RegionType AllImage,
            const typename TImage::RegionType face);

#endif
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVanHerkGilWermanUtilities.txx"
#endif

#endif
