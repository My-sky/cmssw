#ifndef Alignment_SurveyAnalysis_SurveyParameters_h
#define Alignment_SurveyAnalysis_SurveyParameters_h

/** \class SurveyParameters
 *
 *  Alignment parameters for survey.
 *
 *  $Date: 2007/03/14 18:05:35 $
 *  $Revision: 1.1 $
 *  \author Chung Khim Lae
 */

#include "Alignment/CommonAlignment/interface/AlignmentParameters.h"

class SurveyParameters:
  public AlignmentParameters
{
  public:

  /// Set the alignable, parameters, covariance in base class.
  /// No user variables, default is all parameters are selected and valid.
  SurveyParameters(
		   Alignable*,
		   const AlgebraicVector& par,
		   const AlgebraicSymMatrix& cov
		   );

  /// Cloning not implemented.
  virtual AlignmentParameters* clone(
				     const AlgebraicVector&,
                                     const AlgebraicSymMatrix&
				     ) const;

  /// Cloning not implemented.
  virtual AlignmentParameters* cloneFromSelected(
						 const AlgebraicVector&,
                                                 const AlgebraicSymMatrix&
						 ) const;

  /// Derivatives not implemented.
  virtual AlgebraicMatrix derivatives(
				      const TrajectoryStateOnSurface&,
				      const AlignableDetOrUnitPtr&
				      ) const;

  /// Derivatives not implemented.
  virtual AlgebraicMatrix selectedDerivatives(
					      const TrajectoryStateOnSurface&,
					      const AlignableDetOrUnitPtr&
					      ) const;

};

#endif
