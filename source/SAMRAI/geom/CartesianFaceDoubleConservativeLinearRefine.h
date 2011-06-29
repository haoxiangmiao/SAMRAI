/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   Conservative linear refine operator for face-centered
 *                double data on a Cartesian mesh. 
 *
 ************************************************************************/

#ifndef included_geom_CartesianFaceDoubleConservativeLinearRefine
#define included_geom_CartesianFaceDoubleConservativeLinearRefine

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/hier/RefineOperator.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/hier/IntVector.h"
#include "SAMRAI/hier/Patch.h"
#include "SAMRAI/tbox/Pointer.h"

#include <string>

namespace SAMRAI {
namespace geom {

/**
 * Class CartesianFaceDoubleConservativeLinearRefine implements
 * conservative linear interpolation for face-centered double patch data
 * defined over a Cartesian mesh.  It is derived from the base class
 * hier::RefineOperator.  The numerical operations for the interpolation
 * use FORTRAN numerical routines.
 *
 * The findRefineOperator() operator function returns true if the input
 * variable is face-centered double, and the std::string is
 * "CONSERVATIVE_LINEAR_REFINE".
 *
 * @see hier::RefineOperator
 */

class CartesianFaceDoubleConservativeLinearRefine:
   public hier::RefineOperator
{
public:
   /**
    * Uninteresting default constructor.
    */
   explicit CartesianFaceDoubleConservativeLinearRefine(
      const tbox::Dimension& dim);

   /**
    * Uninteresting virtual destructor.
    */
   virtual ~CartesianFaceDoubleConservativeLinearRefine();

   /**
    * Return true if the variable and name std::string match face-centered
    * double conservative linear interpolation; otherwise, return false.
    */
   bool
   findRefineOperator(
      const tbox::Pointer<hier::Variable>& var,
      const std::string& op_name) const;

   /**
    * The priority of face-centered double conservative linear is 0.
    * It will be performed before any user-defined interpolation operations.
    */
   int
   getOperatorPriority() const;

   /**
    * The stencil width of the conservative linear interpolation operator is
    * the vector of ones.
    */
   hier::IntVector
   getStencilWidth() const;

   /**
    * Refine the source component on the coarse patch to the destination
    * component on the fine patch using the face-centered double conservative
    * linear interpolation operator.  Interpolation is performed on the
    * intersection of the destination patch and the boxes contained in
    * fine_overlap.  It is assumed that the coarse patch contains sufficient
    * data for the stencil width of the refinement operator.
    */
   void
   refine(
      hier::Patch& fine,
      const hier::Patch& coarse,
      const int dst_component,
      const int src_component,
      const hier::BoxOverlap& fine_overlap,
      const hier::IntVector& ratio) const;
};

}
}
#endif