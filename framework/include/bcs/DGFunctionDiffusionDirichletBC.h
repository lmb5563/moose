/****************************************************************/
/*               DO NOT MODIFY THIS HEADER                      */
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*           (c) 2010 Battelle Energy Alliance, LLC             */
/*                   ALL RIGHTS RESERVED                        */
/*                                                              */
/*          Prepared by Battelle Energy Alliance, LLC           */
/*            Under Contract No. DE-AC07-05ID14517              */
/*            With the U. S. Department of Energy               */
/*                                                              */
/*            See COPYRIGHT for full restrictions               */
/****************************************************************/

#ifndef DGFUNCTIONDIFFUSIONDIRICHLETBC_H
#define DGFUNCTIONDIFFUSIONDIRICHLETBC_H

#include "BoundaryCondition.h"


//Forward Declarations
class DGFunctionDiffusionDirichletBC;
class Function;

template<>
InputParameters validParams<DGFunctionDiffusionDirichletBC>();

/**
 * Implements a simple BC for DG
 *
 * BC derived from diffusion problem that can handle:
 * { \grad u * n_e} [v] + epsilon { \grad v * n_e } [u] + (sigma / |e| * [u][v])
 *
 *  [a] = [ a_1 - a_2 ]
 *  {a} = 0.5 * (a_1 + a_2)
 */
class DGFunctionDiffusionDirichletBC : public BoundaryCondition
{
public:

  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  DGFunctionDiffusionDirichletBC(std::string name, MooseSystem & moose_system, InputParameters parameters);
    
  virtual ~DGFunctionDiffusionDirichletBC() {}

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

private:
  Function & _func;

  Real _epsilon;
  Real _sigma;
};

#endif //DGFUNCTIONDIFFUSIONDIRICHLETBC_H
