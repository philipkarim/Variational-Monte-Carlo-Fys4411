#include <iostream>
#include "system.h"
#include "particle.h"
#include "WaveFunctions/wavefunction.h"
#include "WaveFunctions/simplegaussian.h"
#include "Hamiltonians/hamiltonian.h"
#include "Hamiltonians/harmonicoscillator.h"
#include "InitialStates/initialstate.h"
#include "InitialStates/randomuniform.h"
#include "Math/random.h"
//#include "mpi.h"

using namespace std;


int main() {

    //Init MPI
/*
    int numprocs, my_rank;
    MPI_Init (&nargs, &args);
    MPI_Comm_size (MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
*/
    // Seed for the random number generator

    int seed = 2021;

    //Dim=1, particle=1 should give 0.5
    //3 10 e5 false true is stable
    //Importance sampling is  stable when timestep=1
    int numberOfDimensions  = 3;
    int numberOfParticles   = 10;
    int numberOfSteps       = (int) 1e5;
    double omega            = 1.0;          // Oscillator frequency.
    double alpha            = 0.7;          // Variational parameter.
    double beta             = 1;
    double timeStep         = 1;            // Metropolis time step (Importance sampling)
    double stepLength       = 1;            // Metropolis step length.
    double equilibration    = 0.2;          // Amount of the total steps used for equilibration.
    bool numeric            = false;         // True->Numeric differentiation, False->Analytic
    bool bruteforce_val     = true;         // True->bruteforce, False->Importance sampling
    bool GD=false;
    double initialAlpha = 0.6;

    System* system = new System(seed);
    system->setHamiltonian              (new HarmonicOscillator(system, omega));  //Added alpha
    system->setWaveFunction             (new SimpleGaussian(system, alpha, beta));
    system->setInitialState             (new RandomUniform(system, numberOfDimensions, numberOfParticles));
    system->setEquilibrationFraction    (equilibration);
    system->setStepLength               (stepLength);
    system->setNumeric                  (numeric);
    system->setBruteforce               (bruteforce_val);
    system->setTimeStep                 (timeStep);

    if (GD==false){

      system->runMetropolisSteps          (numberOfSteps);
    }

    else{
          alpha = system->gradientDescent(initialAlpha);
          vector<double> parameters(2);
          parameters[0] = alpha;
          parameters[1] = beta;
          system->getWaveFunction()->setParameters(parameters);
          system->runMetropolisSteps          (numberOfSteps);

    }
    //system->gradientDecent();

    //system->setNumberOfParticles        (numeric)

    //MPI_Finalize ();

    return 0;
}
