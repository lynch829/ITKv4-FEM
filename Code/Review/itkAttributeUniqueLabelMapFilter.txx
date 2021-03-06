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
#ifndef __itkAttributeUniqueLabelMapFilter_txx
#define __itkAttributeUniqueLabelMapFilter_txx

#include "itkAttributeUniqueLabelMapFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage, class TAttributeAccessor>
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::AttributeUniqueLabelMapFilter()
{
  m_ReverseOrdering = false;
}


template <class TImage, class TAttributeAccessor>
void
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // the priority queue to store all the lines of all the objects sorted
  typedef typename std::priority_queue< LineOfLabelObject, std::vector< LineOfLabelObject >,
                                        LineOfLabelObjectComparator > PriorityQueueType;
  PriorityQueueType pq;

  ProgressReporter progress(this, 0, 1);
  // TODO: really report the progress

  typedef typename ImageType::LabelObjectContainerType LabelObjectsType;

  const LabelObjectsType & labelObjects = this->GetLabelMap()->GetLabelObjectContainer();
  for ( typename LabelObjectsType::const_iterator it2 = labelObjects.begin();
        it2 != labelObjects.end();
        it2++ )
    {
    LabelObjectType *lo = it2->second;

    // may reduce the number of lines to proceed
    lo->Optimize();

    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType & lineContainer = lo->GetLineContainer();

    for ( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      pq.push( LineOfLabelObject(*lit, lo) );
      }

    // clear the lines to readd them later
    lineContainer.clear();

    // go to the next label
    // progress.CompletedPixel();
    }

  if ( pq.empty() )
    {
    // nothing to do
    return;
    }

  typedef typename std::deque< LineOfLabelObject > LinesType;
  LinesType lines;

  lines.push_back( pq.top() );
  LineOfLabelObject prev = lines.back();
  IndexType         prevIdx = prev.line.GetIndex();
  pq.pop();

  AttributeAccessorType accessor;

  while ( !pq.empty() )
    {
    LineOfLabelObject l = pq.top();
    IndexType         idx = l.line.GetIndex();
    pq.pop();

    bool newMainLine = false;
    // don't check dim 0!
    for ( unsigned int i = 1; i < ImageDimension; i++ )
      {
      if ( idx[i] != prevIdx[i] )
        {
        newMainLine = true;
        }
      }

    if ( newMainLine )
      {
      // just push the line
      lines.push_back(l);
      }
    else
      {
      unsigned long prevLength = prev.line.GetLength();
      unsigned long length = l.line.GetLength();

      if ( prevIdx[0] + (long)prevLength >= idx[0] )
        {
        // the lines are overlapping. We need to choose which line to keep.
        // the label, the only "attribute" to be guarenteed to be unique, is
        // used to choose
        // which line to keep. This is necessary to avoid the case where a
        // part of a label is over
        // a second label, and below in another part of the image.
        bool keepCurrent;
        typename TAttributeAccessor::AttributeValueType prevAttr = accessor(prev.labelObject);
        typename TAttributeAccessor::AttributeValueType attr = accessor(l.labelObject);
        // this may be changed to a single boolean expression, but may become
        // quite difficult to read
        if ( attr == prevAttr  )
          {
          if ( l.labelObject->GetLabel() > prev.labelObject->GetLabel() )
            {
            keepCurrent = !m_ReverseOrdering;
            }
          else
            {
            keepCurrent = m_ReverseOrdering;
            }
          }
        else
          {
          if ( attr > prevAttr )
            {
            keepCurrent = !m_ReverseOrdering;
            }
          else
            {
            keepCurrent = m_ReverseOrdering;
            }
          }

        if ( keepCurrent )
          {
          // keep the current one. We must truncate the previous one to remove
          // the
          // overlap, and take care of the end of the previous line if it
          // extends
          // after the current one.
          if ( prevIdx[0] + prevLength > idx[0] + length )
            {
            // the previous line is longer than the current one. Lets take its
            // tail and
            // add it to the priority queue
            IndexType newIdx = idx;
            newIdx[0] = idx[0] + length;
            unsigned long newLength = prevIdx[0] + prevLength - newIdx[0];
            pq.push( LineOfLabelObject(LineType(newIdx, newLength), prev.labelObject) );
            }
          // truncate the previous line to let some place for the current one
          prevLength = idx[0] - prevIdx[0];
          if ( prevLength != 0 )
            {
            lines.back(). line.SetLength(idx[0] - prevIdx[0]);
            }
          else
            {
            // length is 0 - no need to keep that line
            lines.pop_back();
            }
          // and push the current one
          lines.push_back(l);
          }
        else
          {
          // keep the previous one. If the previous line fully overlap the
          // current one,
          // the current one is fully discarded.
          if ( prevIdx[0] + prevLength > idx[0] + length )
            {
            // discarding the current line - just do nothing
            }
          else
            {
            IndexType newIdx = idx;
            newIdx[0] = prevIdx[0] + prevLength;
            unsigned long newLength = idx[0] + length - newIdx[0];
            l.line.SetIndex(newIdx);
            l.line.SetLength(newLength);
            lines.push_back(l);
            }
          }
        }
      else
        {
        // no overlap - things are just fine already
        lines.push_back(l);
        }
      }

    // store the current line as the previous one, and go to the next one.
    prev = lines.back();
    prevIdx = prev.line.GetIndex();
    }

  // put the lines in their object
  for ( unsigned int i = 0; i < lines.size(); i++ )
    {
    LineOfLabelObject & l = lines[i];
    l.labelObject->AddLine(l.line);
    }

  // remove objects without lines
  typename LabelObjectsType::const_iterator it = labelObjects.begin();
  while ( it != labelObjects.end() )
    {
    typename LabelObjectType::LabelType label = it->first;
    LabelObjectType *labelObject = it->second;

    if ( labelObject->Empty() )
      {
      // must increment the iterator before removing the object to avoid
      // invalidating the iterator
      it++;
      this->GetLabelMap()->RemoveLabel(label);
      }
    else
      {
      it++;
      }
    }
}


template <class TImage, class TAttributeAccessor>
void
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
}

}// end namespace itk
#endif
