/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "ThermalContactBCsAction.h"
#include "ThermalContactAuxVarsAction.h"

#include "Factory.h"
#include "FEProblem.h"
#include "MooseApp.h"
#include "Conversion.h"

static unsigned int n = 0;

template<>
InputParameters validParams<ThermalContactBCsAction>()
{
  MooseEnum orders("CONSTANT FIRST SECOND THIRD FOURTH", "FIRST");

  InputParameters params = validParams<Action>();
  params.addParam<std::string>("appended_property_name", "", "Name appended to material properties to make them unique");
  params.addRequiredParam<std::string>("type", "A string representing the Moose object that will be used for heat conduction over the gap");
  params.addRequiredParam<NonlinearVariableName>("variable", "The variable for thermal contact");
  params.addRequiredParam<BoundaryName>("master", "The master surface");
  params.addRequiredParam<BoundaryName>("slave", "The slave surface");
  params.addParam<VariableName>("disp_x", "The x displacement");
  params.addParam<VariableName>("disp_y", "The y displacement");
  params.addParam<VariableName>("disp_z", "The z displacement");
  params.addParam<MooseEnum>("order", orders, "The finite element order");
  params.addParam<bool>("warnings", false, "Whether to output warning messages concerning nodes not being found");
  params.addParam<std::vector<std::string> >("save_in", "The Auxiliary Variable to (optionally) save the boundary flux in");
  params.addParam<bool>("quadrature", false, "Whether or not to use quadrature point based gap heat transfer");

  return params;
}

ThermalContactBCsAction::ThermalContactBCsAction(const InputParameters & params) :
  Action(params)
{
}

void
ThermalContactBCsAction::act()
{
  bool quadrature = getParam<bool>("quadrature");

  InputParameters params = _factory.getValidParams(getParam<std::string>("type"));

  // Extract global params
  _app.parser().extractParams(_name, params);

  if (isParamValid("save_in"))
  {
    params.set<std::vector<std::string> >("save_in") = getParam<std::vector<std::string> >("save_in");
  }

  params.set<NonlinearVariableName>("variable") = getParam<NonlinearVariableName>("variable");

  if (!quadrature)
  {
    std::vector<VariableName> vars(1);
    vars[0] = "penetration";
    params.set<std::vector<VariableName> >("gap_distance") = vars;
    vars[0] = ThermalContactAuxVarsAction::getGapValueName(_pars);
    params.set<std::vector<VariableName> >("gap_temp") = vars;
  }
  else
  {
    params.set<bool>("quadrature") = true;
    params.set<BoundaryName>("paired_boundary") = getParam<BoundaryName>("master");
    params.set<MooseEnum>("order") = getParam<MooseEnum>("order");
    params.set<bool>("warnings") = getParam<bool>("warnings");
    params.set<bool>("use_displaced_mesh") = true;
  }

  std::vector<BoundaryName> bnds(1, getParam<BoundaryName>("slave"));
  params.set<std::vector<BoundaryName> >("boundary") = bnds;

  params.set<std::string>("appended_property_name") = getParam<std::string>("appended_property_name");

  params.addCoupledVar("disp_x", "The x displacement");
  params.addCoupledVar("disp_y", "The y displacement");
  params.addCoupledVar("disp_z", "The z displacement");

  if (isParamValid("disp_x"))
  {
    std::vector<VariableName> disp_x(1, getParam<VariableName>("disp_x"));
    params.set< std::vector<VariableName> >("disp_x") = disp_x;
  }
  if (isParamValid("disp_y"))
  {
    std::vector<VariableName> disp_y(1, getParam<VariableName>("disp_y"));
    params.set< std::vector<VariableName> >("disp_y") = disp_y;
  }
  if (isParamValid("disp_z"))
  {
    std::vector<VariableName> disp_z(1, getParam<VariableName>("disp_z"));
    params.set< std::vector<VariableName> >("disp_z") = disp_z;
  }

  _problem->addBoundaryCondition(getParam<std::string>("type"),
      "gap_bc_" + Moose::stringify(n),
      params);

  if (quadrature)
  {
    // Swap master and slave for this one
    std::vector<BoundaryName> bnds(1, getParam<BoundaryName>("master"));
    params.set<std::vector<BoundaryName> >("boundary") = bnds;
    params.set<BoundaryName>("paired_boundary") = getParam<BoundaryName>("slave");

    _problem->addBoundaryCondition(getParam<std::string>("type"),
        "gap_bc_master_" + Moose::stringify(n),
        params);
  }


  ++n;
}

