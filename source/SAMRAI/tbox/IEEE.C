/*************************************************************************
 *
 * This file is part of the SAMRAI distribution.  For full copyright 
 * information, see COPYRIGHT and COPYING.LESSER. 
 *
 * Copyright:     (c) 1997-2010 Lawrence Livermore National Security, LLC
 * Description:   IEEE routines to set up handlers and get signaling NaNs 
 *
 ************************************************************************/

#include "SAMRAI/tbox/IEEE.h"

#include "SAMRAI/tbox/SAMRAI_MPI.h"
#include "SAMRAI/tbox/MathUtilities.h"

/*
 * Floating point exception handling.
 *
 * The following lines setup exception handling headers.
 */
#if defined(HAVE_EXCEPTION_HANDLING)
#include <fpu_control.h>
#include <signal.h>
#endif

/*
 * The following lines setup exception handling headers on the Sun.  If we
 * use Sun's native compiler, just pull in the <sunmath.h> include file.
 * If we are under solaris but use a different compiler (e.g. g++)
 * we have to explicitly define the functions that <sunmath.h> defines,
 * since we don't have access to this file.
 */
#ifdef __SUNPRO_CC
#include <sunmath.h>
#endif

#ifndef SAMRAI_INLINE
#include "SAMRAI/tbox/IEEE.I"
#endif

namespace SAMRAI {
namespace tbox {

/*
 *************************************************************************
 * Set up the IEEE exception handlers so that normal IEEE exceptions	*
 * will cause a program abort.  How this is done varies wildly from	*
 * architecture to architecture.                                        *
 *************************************************************************
 */

/*
 * Function celled when an exception is tripped.
 */
#if defined(HAVE_EXCEPTION_HANDLING)
static void error_action(
   int error)
{
   Utilities::abort(
      "Floating point exception -- program abort! "
      + tbox::Utilities::intToString(error),
      __FILE__,
      __LINE__);
}
#endif

void IEEE::setupFloatingPointExceptionHandlers()
{
#if defined(HAVE_EXCEPTION_HANDLING)
   int fpu_flags = _FPU_DEFAULT;
   fpu_flags &= ~_FPU_MASK_IM;  /* Execption on Invalid operation */
   fpu_flags &= ~_FPU_MASK_ZM;  /* Execption on Division by zero  */
   fpu_flags &= ~_FPU_MASK_OM;  /* Execption on Overflow */
   _FPU_SETCW(fpu_flags);
   signal(SIGFPE, error_action);
#endif
}

/*
 *************************************************************************
 *									*
 * Routines to initialize arrays to signaling NaNs.                      *
 *									*
 *************************************************************************
 */

void IEEE::initializeArrayToSignalingNaN(
   Array<float>& array)
{
   MathUtilities<float>::setArrayToSignalingNaN(array);
}

void IEEE::initializeArrayToSignalingNaN(
   Array<double>& array)
{
   MathUtilities<double>::setArrayToSignalingNaN(array);
}

void IEEE::initializeArrayToSignalingNaN(
   Array<dcomplex>& array)
{
   MathUtilities<dcomplex>::setArrayToSignalingNaN(array);
}

void IEEE::initializeArrayToSignalingNaN(
   float* array,
   int n)
{
   MathUtilities<float>::setArrayToSignalingNaN(array, n);
}

void IEEE::initializeArrayToSignalingNaN(
   double* array,
   int n)
{
   MathUtilities<double>::setArrayToSignalingNaN(array, n);
}

void IEEE::initializeArrayToSignalingNaN(
   dcomplex* array,
   int n)
{
   MathUtilities<dcomplex>::setArrayToSignalingNaN(array, n);
}

}
}