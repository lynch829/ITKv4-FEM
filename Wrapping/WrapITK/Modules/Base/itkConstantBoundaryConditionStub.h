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
#ifndef CABLE_CONFIGURATION
// This file should *only* ever be included on windows systems!
// gcc-xml cannot be allowed to see the full ConstantBoundaryCondition header.
// If we're not gcc_xml, just pass through to the proper header.
#include "itkConstantBoundaryCondition.h"
#else
// Here follows the restricted header made available just to gcc-xml on windows.
// The problem we are working around is the fact that on windows, the
// NumericTraits<PixelType>::Zero values are declared as __declspec(dllimport),
// which gcc-xml and cswig can't deal with properly. So we hide that stuff in
// these restricted headers.

#ifndef __itkConstantBoundaryCondition_h
#define __itkConstantBoundaryCondition_h

namespace itk
{
template<class TImage>
class ITK_EXPORT ConstantBoundaryCondition
  : public ImageBoundaryCondition<TImage>
{
public:
  typedef ConstantBoundaryCondition Self;
  typedef ImageBoundaryCondition<TImage> Superclass;
  typedef typename Superclass::PixelType PixelType;
  typedef typename Superclass::PixelPointerType PixelPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::OffsetType OffsetType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;

  typedef typename Superclass::NeighborhoodAccessorFunctorType
                                 NeighborhoodAccessorFunctorType;

  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  ConstantBoundaryCondition();
  virtual PixelType operator()(const OffsetType&,
                               const OffsetType&,
                               const NeighborhoodType *) const;
  virtual PixelType operator()(
      const OffsetType& ,
      const OffsetType& ,
      const NeighborhoodType *,
      const NeighborhoodAccessorFunctorType & ) const;
  void SetConstant(const PixelType &c);
  /** Get the value of the constant. */
  const PixelType &GetConstant() const;
};

} // end namespace itk

#endif // ifndef stuff
#endif // end if CABLE_CONFIGURATION
