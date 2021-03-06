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
#include "itkScaleLogarithmicTransform.h"

#ifdef CABLE_CONFIGURATION
#include "itkCSwigMacros.h"

#define ITK_WRAP_TRANSFORM_2(x, d1) \
  ITK_WRAP_OBJECT2(x, double, d1, itk##x##d1)

namespace _cable_
{
  const char* const group = ITK_WRAP_GROUP(itkScaleTransform);
  namespace wrappers
  {
    ITK_WRAP_TRANSFORM_2(ScaleTransform, 2);
    ITK_WRAP_TRANSFORM_2(ScaleTransform, 3);
    ITK_WRAP_TRANSFORM_2(ScaleLogarithmicTransform, 2);
    ITK_WRAP_TRANSFORM_2(ScaleLogarithmicTransform, 3);
  }
}
#endif
