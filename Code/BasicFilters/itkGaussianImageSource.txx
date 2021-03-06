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
#ifndef __itkGaussianImageSource_txx
#define __itkGaussianImageSource_txx

#include "itkGaussianImageSource.h"
#include "itkGaussianSpatialFunction.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkObjectFactory.h"

namespace itk
{
template< class TOutputImage >
GaussianImageSource< TOutputImage >
::GaussianImageSource()
{
  //Initial image is 64 wide in each direction.
  for ( unsigned int i = 0; i < TOutputImage::GetImageDimension(); i++ )
    {
    m_Size[i] = 64;
    m_Spacing[i] = 1.0;
    m_Origin[i] = 0.0;
    }
  m_Direction.SetIdentity();

  // Gaussian parameters, defined so that the gaussian
  // is centered in the default image
  m_Mean.Fill(32.0);
  m_Sigma.Fill(16.0);
  m_Scale = 255.0;
  m_Normalized = false;
}

template< class TOutputImage >
GaussianImageSource< TOutputImage >
::~GaussianImageSource()
{}

template< class TOutputImage >
void
GaussianImageSource< TOutputImage >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  unsigned int i;

  os << indent << "Size: [";
  for ( i = 0; i < NDimensions - 1; i++ )
    {
    os << m_Size[i] << ", ";
    }
  os << "]" << std::endl;

  os << indent << "Origin: [";
  for ( i = 0; i < NDimensions - 1; i++ )
    {
    os << m_Origin[i] << ", ";
    }
  os << "]" << std::endl;

  os << indent << "Spacing: " << m_Spacing << std::endl;

  os << indent << "Direction:" << std::endl;
  os << m_Direction << std::endl;

  os << indent << "Gaussian sigma: [";
  for ( i = 0; i < NDimensions - 1; i++ )
    {
    os << m_Sigma[i] << ", ";
    }
  os << "]" << std::endl;

  os << indent << "Gaussian mean: [";
  for ( i = 0; i < NDimensions - 1; i++ )
    {
    os << m_Mean[i] << ", ";
    }
  os << "]" << std::endl;

  os << indent << "Gaussian scale: " << m_Scale << std::endl;
  os << indent << "Normalized Gaussian?: " << m_Normalized << std::endl;
}

//----------------------------------------------------------------------------
template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::GenerateOutputInformation()
{
  TOutputImage *output;

  typename TOutputImage::IndexType index = { { 0 } };
  typename TOutputImage::SizeType size = { { 0 } };
  size.SetSize(m_Size);

  output = this->GetOutput(0);

  typename TOutputImage::RegionType largestPossibleRegion;
  largestPossibleRegion.SetSize(size);
  largestPossibleRegion.SetIndex(index);
  output->SetLargestPossibleRegion(largestPossibleRegion);

  output->SetSpacing(m_Spacing);
  output->SetOrigin(m_Origin);
  output->SetDirection(m_Direction);
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::GenerateData()
{
  typename TOutputImage::Pointer outputPtr = this->GetOutput();

  // allocate the output buffer
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();

  // Create and initialize a new gaussian function
  typedef itk::GaussianSpatialFunction< double, NDimensions > FunctionType;
  typedef typename FunctionType::InputType                    FunctionPositionType;
  typename FunctionType::Pointer pGaussian = FunctionType::New();

  pGaussian->SetSigma(m_Sigma);
  pGaussian->SetMean(m_Mean);
  pGaussian->SetScale(m_Scale);
  pGaussian->SetNormalized(m_Normalized);

  // Create an iterator that will walk the output region
  typedef ImageRegionIterator< TOutputImage > OutputIterator;
  OutputIterator outIt = OutputIterator( outputPtr,
                                         outputPtr->GetRequestedRegion() );

  // The value produced by the spatial function
  double value;

  // The position at which the function is evaluated
  Point< double, TOutputImage::ImageDimension > evalPoint;

  ProgressReporter progress( this, 0,
                             outputPtr->GetRequestedRegion()
                             .GetNumberOfPixels() );
  // Walk the output image, evaluating the spatial function at each pixel
  for (; !outIt.IsAtEnd(); ++outIt )
    {
    typename TOutputImage::IndexType index = outIt.GetIndex();
    outputPtr->TransformIndexToPhysicalPoint(index, evalPoint);
    value = pGaussian->Evaluate(evalPoint);

    // Set the pixel value to the function value
    outIt.Set( ( typename TOutputImage::PixelType )value );
    progress.CompletedPixel();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetSpacing(const float *spacing)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( (double)spacing[i] != m_Spacing[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Spacing[i] = spacing[i];
      }
    this->Modified();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetSpacing(const double *spacing)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( spacing[i] != m_Spacing[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Spacing[i] = spacing[i];
      }
    this->Modified();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetOrigin(const float *origin)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( (double)origin[i] != m_Origin[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Origin[i] = origin[i];
      }
    this->Modified();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetOrigin(const double *origin)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( origin[i] != m_Origin[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Origin[i] = origin[i];
      }
    this->Modified();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetSize(const SizeValueType *size)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( size[i] != m_Size[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Size[i] = size[i];
      }
    this->Modified();
    }
}

template< typename TOutputImage >
void
GaussianImageSource< TOutputImage >
::SetSize(const SizeType size)
{
  unsigned int i;

  for ( i = 0; i < TOutputImage::ImageDimension; i++ )
    {
    if ( size[i] != m_Size[i] )
      {
      break;
      }
    }
  if ( i < TOutputImage::ImageDimension )
    {
    for ( i = 0; i < TOutputImage::ImageDimension; i++ )
      {
      m_Size[i] = size[i];
      }
    this->Modified();
    }
}
} // end namespace itk

#endif
