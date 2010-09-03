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

#ifndef TRANSIENTEXECUTIONER_H
#define TRANSIENTEXECUTIONER_H

#include "Moose.h"
#include "Executioner.h"

// LibMesh includes
#include <parameters.h>
#include <point.h>
#include <vector_value.h>

// System includes
#include <string>

// Forward Declarations
class TransientExecutioner;
template<>
InputParameters validParams<TransientExecutioner>();

/**
 * TransientExecutioner executioners usually loop through a number of timesteps... calling solve()
 * for each timestep.
 */
class TransientExecutioner: public Executioner
{
public:

  /**
   * Constructor
   *
   * @param name The name given to the Executioner in the input file.
   * @param parameters The parameters object holding data for the class to use.
   * @return Whether or not the solve was successful.
   */
  TransientExecutioner(std::string name, MooseSystem & moose_system, InputParameters parameters);

  /**
   * This will call solve() on the NonlinearSystem.
   */
  virtual void execute();

  /**
   * This should execute the solve for one timestep.
   */
  virtual void takeStep();

  /**
   * Optional override.
   * 
   * @return The dt to use for this timestep.
   */
  virtual Real computeDT();

  /**
   * Transient loop will continue as long as this keeps returning true.
   */
  virtual bool keepGoing();

  /**
   * Whether or not the last solve converged.
   */
  virtual bool lastSolveConverged();

protected:

  /**
   * Current timestep.
   *
   * Please don't modify this directly!
   */
  int & _t_step;

  /**
   * Current time.
   *
   * Please don't modify this directly!
   */
  double & _time;

  /**
   * The dt from the input file.
   */
  double _input_dt;
  
  /**
   * Current delta t... or timestep size.
   *
   * Please don't modify this directly!
   */
  double & _dt;

  double _end_time;
  double _dtmin;
  double _dtmax;
  int _num_steps;
  int _n_startup_steps;

  /**
   * Steady state detection variables:
   */
  bool _trans_ss_check;
  double _ss_check_tol;
  double _ss_tmin;
  Real _old_time_solution_norm;
  
  /**
   * Whether or not the previous solve converged.
   */
  bool _converged;
};

#endif //TRANSIENTEXECUTIONER_H
