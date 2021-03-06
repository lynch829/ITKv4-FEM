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
#ifndef __itkKernelImageFilter_txx
#define __itkKernelImageFilter_txx

#include "itkKernelImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk
{
template< class TInputImage, class TOutputImage, class TKernel >
KernelImageFilter< TInputImage, TOutputImage, TKernel >
::KernelImageFilter()
{
  this->SetRadius(1UL);
}

template< class TInputImage, class TOutputImage, class TKernel >
void
KernelImageFilter< TInputImage, TOutputImage, TKernel >
::SetRadius(const RadiusType & radius)
{
  // Try to use a FlatStructuringElement if possible, because it is
  // much efficient with van Herk / Gil Werman filters
  // check that the type is exactly FlatKernelType, to be sure to
  // not replace m_Kernel data with the data of a FlatKernelType
  // if KernelType is a subclass of FlatKernelType.
  if ( typeid( KernelType ) == typeid( FlatKernelType ) )
    {
    // SetKernel() must be called, because it can be overloaded in a subclass
    // - MovingHistogramImageFilterBase for example.
    FlatKernelType flatKernel = FlatKernelType::Box(radius);
    KernelType *   kernel = reinterpret_cast< KernelType * >( &flatKernel );
    this->SetKernel(*kernel);
    }
  else
    {
    // Superclass::SetRadius() is called in SetKenel() - no need to call it here
    // Superclass::SetRadius( radius );
    KernelType kernel;
    kernel.SetRadius(radius);
    for ( typename KernelType::Iterator kit = kernel.Begin(); kit != kernel.End(); kit++ )
      {
      *kit = 1;
      }
    this->SetKernel(kernel);
    }
}

template< class TInputImage, class TOutputImage, class TKernel >
void
KernelImageFilter< TInputImage, TOutputImage, TKernel >
::SetKernel(const KernelType & kernel)
{
  if ( m_Kernel != kernel )
    {
    m_Kernel = kernel;
    this->Modified();
    }
  // set the radius of the super class to be the same than the kernel one
  Superclass::SetRadius( kernel.GetRadius() );
}

template< class TInputImage, class TOutputImage, class TKernel >
void
KernelImageFilter< TInputImage, TOutputImage, TKernel >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Kernel: " << m_Kernel << std::endl;
}
}

#endif
