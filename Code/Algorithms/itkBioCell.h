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
#ifndef __itkBioCell_h
#define __itkBioCell_h

#include "itkBioCellBase.h"
#include "itkBioCellularAggregateBase.h"

namespace itk
{
namespace bio
{
/** \class Cell
 * \brief This class implement the minimal behavior
 * of a biological cell.
 * The basic behavior of a cell is related with the
 * cell cycle. Geometrical concepts like size and shape
 * are also managed by this abstract cell.
 */

template< unsigned int NSpaceDimension = 3 >
class ITK_EXPORT Cell:public CellBase
{
public:
  typedef   CellBase Superclass;

  typedef   itk::Vector< double, NSpaceDimension > VectorType;
  typedef   itk::Point< double, NSpaceDimension >  PointType;

  friend class CellularAggregateBase; // need to give access to the constructor.
public:
  virtual ~Cell();
  virtual void ClearForce(void);

  virtual void AddForce(const VectorType & force);

  virtual void AdvanceTimeStep(void);

  virtual void Mitosis(void);

  virtual void Apoptosis(void);

  virtual void ReceptorsReading(void);

  virtual void SetCellularAggregate(CellularAggregateBase *);

  virtual CellularAggregateBase * GetCellularAggregate(void);

  virtual const CellularAggregateBase * GetCellularAggregate(void) const;

  static const char * GetSpeciesName(void)
  { return "Primitive Cell"; }
  static Cell * CreateEgg(void);

  static unsigned int GetDimension()
  { return NSpaceDimension; }
protected:
  Cell(); // Users should create cell with the CreateEgg() method
public:

  virtual const VectorType & GetForce(void) const;

protected:
  VectorType m_Force;

  CellularAggregateBase *m_Aggregate;
};
} // end namespace bio
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBioCell.txx"
#endif

#endif
