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
#ifndef __itkSpatialObjectToPointSetFilter_h
#define __itkSpatialObjectToPointSetFilter_h

#include "itkPointSet.h"
#include "itkMeshSource.h"
#include "itkPointBasedSpatialObject.h"

namespace itk
{
/** \class SpatialObjectToPointSetFilter
 * \brief Base class for filters that take a SpatialObject
 *        as input and produce a PointSet as output.
 *  The pointset created is in physical space.
 */
template< class TInputSpatialObject, class TOutputPointSet >
class ITK_EXPORT SpatialObjectToPointSetFilter:public MeshSource< TOutputPointSet >
{
public:
  /** Standard class typedefs. */
  typedef SpatialObjectToPointSetFilter Self;
  typedef MeshSource< TOutputPointSet > Superclass;
  typedef SmartPointer< Self >          Pointer;
  typedef SmartPointer< const Self >    ConstPointer;

  typedef TOutputPointSet                      OutputPointSetType;
  typedef typename OutputPointSetType::Pointer OutputPointSetPointer;

  /** Smart Pointer type to a DataObject. */
  typedef DataObject::Pointer DataObjectPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SpatialObjectToPointSetFilter, ProcessObject);

  /** Some convenient typedefs. */
  typedef TInputSpatialObject                            InputSpatialObjectType;
  typedef typename InputSpatialObjectType::Pointer       InputSpatialObjectPointer;
  typedef typename InputSpatialObjectType::ConstPointer  InputSpatialObjectConstPointer;
  typedef typename TInputSpatialObject::ChildrenListType ChildrenListType;

  /** Dimension constants */
  itkStaticConstMacro(ObjectDimension, unsigned int,
                      InputSpatialObjectType::ObjectDimension);

  typedef itk::SpatialObjectPoint< itkGetStaticConstMacro(ObjectDimension) >      PointType;
  typedef itk::PointBasedSpatialObject< itkGetStaticConstMacro(ObjectDimension) > PointBasedSpatialObjectType;

  /** Set/Get the PointSet input of this process object.  */
  virtual void SetInput(const InputSpatialObjectType *object);

  virtual void SetInput(unsigned int, const InputSpatialObjectType *object);

  const InputSpatialObjectType * GetInput(void);

  const InputSpatialObjectType * GetInput(unsigned int idx);

  /** The spatial object being transformed can be part of a hierarchy.
   * How deep in the hierarchy should we descend in generating the
   * PointSet?  A ChildrenDepth of 0 means to only include the object
   * itself. */
  itkSetMacro(ChildrenDepth, unsigned int);
  itkGetConstMacro(ChildrenDepth, unsigned int);

  /* Set the sampling factor of the object. The resulting pointset will have a size
   * inversely proportional to the sampling factor.*/
  itkSetMacro(SamplingFactor, unsigned int);
  itkGetConstMacro(SamplingFactor, unsigned int);
protected:
  SpatialObjectToPointSetFilter();
  ~SpatialObjectToPointSetFilter();

  virtual void GenerateOutputInformation(){}  // do nothing
  virtual void GenerateData();

  unsigned int m_ChildrenDepth;

  virtual void PrintSelf(std::ostream & os, Indent indent) const;

private:
  SpatialObjectToPointSetFilter(const Self &); //purposely not implemented
  void operator=(const Self &);                //purposely not implemented

  unsigned int m_SamplingFactor; //default 1
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSpatialObjectToPointSetFilter.txx"
#endif

#endif
