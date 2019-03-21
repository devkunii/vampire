//------------------------------------------------------------------------------
//
//   This file is part of the VAMPIRE open source package under the
//   Free BSD licence (see licence file for details).
//
//   (c) Richard F L Evans 2019. All rights reserved.
//
//   Email: richard.evans@york.ac.uk
//
//------------------------------------------------------------------------------
//

// C++ standard library headers

// Vampire headers
#include "atoms.hpp"
#include "cells.hpp"
#include "create.hpp"
#include "dipole.hpp"
#include "ltmp.hpp"
#include "sim.hpp"
#include "spintorque.hpp"
#include "spintransport.hpp"
#include "unitcell.hpp"
#include "vio.hpp"
#include "vmpi.hpp"

// sim namespace
namespace sim{

// sim::internal namespace
namespace internal{

//------------------------------------------------------------------------------
// Simple wrapper function to initialise vampire modules
//------------------------------------------------------------------------------
//
//   Each module should only self-initialise if necessary
//   (eg an input parameter depends on it)
//
//   Note initialisation order here is **important**:
//        some modules have dependencies on other modules
//
//------------------------------------------------------------------------------
void initialize_modules(){

   zlog << zTs() << "Initializing vampire modules" << std::endl;

   // Determine number of local atoms
   #ifdef MPICF
      const int num_local_atoms = vmpi::num_core_atoms+vmpi::num_bdry_atoms;
   #else
      const int num_local_atoms = atoms::num_atoms;
   #endif

   //----------------------------------------
   // Initialise cells data
   //----------------------------------------
   cells::initialize(cs::system_dimensions[0],
                     cs::system_dimensions[1],
                     cs::system_dimensions[2],
                     cs::unit_cell.dimensions[0],
                     cs::unit_cell.dimensions[1],
                     cs::unit_cell.dimensions[2],
                     atoms::x_coord_array,
                     atoms::y_coord_array,
                     atoms::z_coord_array,
                     atoms::type_array,
                     atoms::cell_array,
						   create::num_total_atoms_non_filler,
                     atoms::num_atoms
      );

   //----------------------------------------
   // Initialise spin torque data
   //----------------------------------------
   st::initialise(cs::system_dimensions[0],
                  cs::system_dimensions[1],
                  cs::system_dimensions[2],
                  atoms::x_coord_array,
                  atoms::y_coord_array,
                  atoms::z_coord_array,
                  atoms::type_array,
                  num_local_atoms);

   //----------------------------------------
   // Initialise local temperature data
   //----------------------------------------
   ltmp::initialise(cs::system_dimensions[0],
                  cs::system_dimensions[1],
                  cs::system_dimensions[2],
                  atoms::x_coord_array,
                  atoms::y_coord_array,
                  atoms::z_coord_array,
                  atoms::type_array,
                  num_local_atoms,
                  sim::Teq,
                  sim::pump_power,
                  sim::pump_time,
                  sim::TTG,
                  sim::TTCe,
                  sim::TTCl,
                  mp::dt_SI,
					   sim::Tmin,
					   sim::Tmax);

   //---------------------------------------------------------------------------
   // Spin transport module
   //---------------------------------------------------------------------------
   spin_transport::initialize(cs::system_dimensions[0],
                              cs::system_dimensions[1],
                              cs::system_dimensions[2],
                              mp::num_materials,
                              atoms::num_atoms,
                              atoms::type_array,
                              atoms::x_coord_array,
                              atoms::y_coord_array,
                              atoms::z_coord_array,
                              atoms::m_spin_array,
                              cs::non_magnetic_atoms_array);

   return;

}

} // end of internal namespace
} // end of sim namespace
