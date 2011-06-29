/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   Norm operations for complex face-centered data. 
 *
 ************************************************************************/

#ifndef included_math_PatchFaceDataNormOpsComplex
#define included_math_PatchFaceDataNormOpsComplex

#include "SAMRAI/SAMRAI_config.h"

#include "SAMRAI/pdat/FaceData.h"
#include "SAMRAI/math/ArrayDataNormOpsComplex.h"
#include "SAMRAI/hier/Box.h"
#include "SAMRAI/tbox/Complex.h"
#include "SAMRAI/tbox/Pointer.h"

namespace SAMRAI {
namespace math {

/**
 * Class PatchFaceDataNormOpsComplex provides a collection of common
 * norm operations that may be applied to complex face-centered patch data.
 * The primary intent of this class is to define part of the interface for the
 * PatchFaceDataOpsComplex class which provides access to operations
 * that may be used to manipulate complex face-centered patch data.  Each
 * member function accepts a box argument indicating the region of index space
 * on which the operation should be performed.  The operation will be performed
 * on the intersection of this box and those boxes corresponding to the patch
 * data objects.  Also, each operation allows an additional face-centered
 * patch data object to be used to represent a control volume that weights the
 * contribution of each data entry in the given norm calculation.  Note that
 * the control volume patch data must be of type double and have face-centered
 * geometry (i.e., the same as the data itself).  The use of control volumes
 * is important when these operations are used in vector kernels where the
 * data resides over multiple levels of spatial resolution in an AMR hierarchy.
 * If the control volume is not given in the function call, it will be ignored
 * in the calculation.  Also, note that the depth of the control volume patch
 * data object must be either 1 or be equal to the depth of the other data
 * objects.
 *
 * Note that a similar set of norm operations is implemented for real
 * patch data (double and float) in the class PatchFaceDataNormOpsReal.
 *
 * @see math::ArrayDataNormOpsComplex
 */

class PatchFaceDataNormOpsComplex
{
public:
   /**
    * Empty constructor and destructor.
    */
   PatchFaceDataNormOpsComplex();

   virtual ~PatchFaceDataNormOpsComplex();

   /**
    * Return the number of data values for the face-centered data object
    * in the given box.  Note that it is assumed that the box refers to
    * the cell-centered index space corresponding to the patch hierarchy.
    */
   int
   numberOfEntries(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box) const;

   /**
    * Return sum of control volume entries for the face-centered data object.
    */
   double
   sumControlVolumes(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const tbox::Pointer<pdat::FaceData<double> >& cvol,
      const hier::Box& box) const;

   /**
    * Set destination component to norm of source component.  That is,
    * each destination entry is set to
    * \f$d_i = \sqrt{ {real(s_i)}^2 + {imag(s_i)}^2 }\f$.
    */
   void
   abs(
      tbox::Pointer<pdat::FaceData<double> >& dst,
      const tbox::Pointer<pdat::FaceData<dcomplex> >& src,
      const hier::Box& box) const;

   /**
    * Return discrete \f$L_1\f$-norm of the data using the control volume to
    * weight the contribution of each data entry to the sum.  That is, the
    * return value is the sum \f$\sum_i ( \sqrt{data_i * \bar{data_i}}*cvol_i )\f$.
    * If the control volume is NULL, the return value is
    * \f$\sum_i ( \sqrt{data_i * \bar{data_i}} )\f$.
    */
   double
   L1Norm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return discrete \f$L_2\f$-norm of the data using the control volume to
    * weight the contribution of each data entry to the sum.  That is, the
    * return value is the sum
    * \f$\sqrt{ \sum_i ( data_i * \bar{data_i} cvol_i ) }\f$.
    * If the control volume is NULL, the return value is
    * \f$\sqrt{ \sum_i ( data_i * \bar{data_i} ) }\f$.
    */
   double
   L2Norm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return discrete weighted \f$L_2\f$-norm of the data using the control
    * volume to weight the contribution of the data and weight entries to
    * the sum.  That is, the return value is the sum \f$\sqrt{ \sum_i (
    * (data_i * wgt_i) * \bar{(data_i * wgt_i)} cvol_i ) }\f$.  If the control
    * volume is NULL, the return value is
    * \f$\sqrt{ \sum_i ( (data_i * wgt_i) * \bar{(data_i * wgt_i)} cvol_i ) }\f$.
    */
   double
   weightedL2Norm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const tbox::Pointer<pdat::FaceData<dcomplex> >& weight,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return discrete root mean squared norm of the data.  If the control
    * volume is not NULL, the return value is the \f$L_2\f$-norm divided by
    * the square root of the sum of the control volumes.  Otherwise, the
    * return value is the \f$L_2\f$-norm divided by the square root of the
    * number of data entries.
    */
   double
   RMSNorm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return discrete weighted root mean squared norm of the data.  If the
    * control volume is not NULL, the return value is the weighted \f$L_2\f$-norm
    * divided by the square root of the sum of the control volumes.  Otherwise,
    * the return value is the weighted \f$L_2\f$-norm divided by the square root
    * of the number of data entries.
    */
   double
   weightedRMSNorm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const tbox::Pointer<pdat::FaceData<dcomplex> >& weight,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return the \f$\max\f$-norm of the data using the control volume to weight
    * the contribution of each data entry to the maximum.  That is, the return
    * value is \f$\max_i ( \sqrt{data_i * \bar{data_i}} )\f$, where the max is
    * over the data elements where \f$cvol_i > 0\f$.  If the control volume is
    * NULL, it is ignored during the computation of the maximum.
    */
   double
   maxNorm(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return the dot product of the two data arrays using the control volume
    * to weight the contribution of each product to the sum.  That is, the
    * return value is the sum \f$\sum_i ( data1_i * \bar{data2_i} * cvol_i )\f$.
    * If the control volume is NULL, it is ignored during the summation.
    */
   dcomplex
   dot(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data1,
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data2,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > cvol =
         tbox::Pointer<pdat::FaceData<double> >(NULL)) const;

   /**
    * Return the integral of the function represented by the data array.
    * The return value is the sum \f$\sum_i ( data_i * vol_i )\f$.
    */
   dcomplex
   integral(
      const tbox::Pointer<pdat::FaceData<dcomplex> >& data,
      const hier::Box& box,
      const tbox::Pointer<pdat::FaceData<double> > vol) const;

private:
   // The following are not implemented:
   PatchFaceDataNormOpsComplex(
      const PatchFaceDataNormOpsComplex&);
   void
   operator = (
      const PatchFaceDataNormOpsComplex&);

   ArrayDataNormOpsComplex d_array_ops;
};

}
}
#endif