/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   Operations for integer cell-centered patch data. 
 *
 ************************************************************************/

#ifndef included_math_PatchCellDataOpsInteger
#define included_math_PatchCellDataOpsInteger

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/CellData.h"
#include "SAMRAI/math/PatchCellDataBasicOps.h"
#include "SAMRAI/math/ArrayDataNormOpsInteger.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/tbox/PIO.h"
#include "SAMRAI/tbox/Pointer.h"

#include <iostream>

namespace SAMRAI {
namespace math {

/**
 * Class PatchCellDataOpsInteger provides a collection of operations
 * that may be used to manipulate integer cell-centered patch data.  The
 * operations include basic arithmetic, min, max, etc.  With the assertion
 * of a few basic routines, this class inherits its interface (and
 * thus its functionality) from the base class PatchCellDataBasicOps
 * from which it is derived.
 *
 * A more extensive set of operations is implemented for real (double and
 * float) and complex patch data in the classes PatchCellDataOpsReal
 * and PatchCellDataOpsComplex, respectively.
 *
 * @see math::PatchCellDataBasicOps
 */

class PatchCellDataOpsInteger:
   public tbox::DescribedClass,
   public PatchCellDataBasicOps<int>
{
public:
   /**
    * Empty constructor and destructor.
    */
   PatchCellDataOpsInteger();

   virtual ~PatchCellDataOpsInteger();

   /**
    * Return the number of data values for the cell-centered data object
    * in the given box.
    */
   int
   numberOfEntries(
      const tbox::Pointer<pdat::CellData<int> >& data,
      const hier::Box& box) const;

   /**
    * Copy dst data to src data over given box.
    */
   void
   copyData(
      tbox::Pointer<pdat::CellData<int> >& dst,
      const tbox::Pointer<pdat::CellData<int> >& src,
      const hier::Box& box) const;

   /**
    * Swap pointers for patch data objects.  Objects are checked for
    * consistency of depth, box, and ghost box.
    */
   void
   swapData(
      tbox::Pointer<hier::Patch> patch,
      const int data1_id,
      const int data2_id) const;

   /**
    * Print data entries over given box to given output stream.
    */
   void
   printData(
      const tbox::Pointer<pdat::CellData<int> >& data,
      const hier::Box& box,
      std::ostream& s = tbox::plog) const;

   /**
    * Initialize data to given scalar over given box.
    */
   void
   setToScalar(
      tbox::Pointer<pdat::CellData<int> >& dst,
      const int& alpha,
      const hier::Box& box) const;

   /**
    * Set destination component to absolute value of source component.
    * That is, each destination entry is set to \f$d_i = \| s_i \|\f$.
    */
   void
   abs(
      tbox::Pointer<pdat::CellData<int> >& dst,
      const tbox::Pointer<pdat::CellData<int> >& src,
      const hier::Box& box) const;

private:
   // The following are not implemented:
   PatchCellDataOpsInteger(
      const PatchCellDataOpsInteger&);
   void
   operator = (
      const PatchCellDataOpsInteger&);

   ArrayDataNormOpsInteger d_array_ops;

};

}
}
#endif