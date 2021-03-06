/*! \file allvars.h
 *  \brief declares global variables.
 *
 *  This file declares all global variables. Further variables should be added here, and declared as
 *  'extern'. The actual existence of these variables is provided by the file 'allvars.c'. To produce
 *  'allvars.c' from 'allvars.h', do the following:
 *
 *     - Erase all #define statements
 *     - add #include "allvars.h"
 *     - delete all keywords 'extern'
 *     - delete all struct definitions enclosed in {...}, e.g.
 *        "extern struct global_data_all_processes {....} All;"
 *        becomes "struct global_data_all_processes All;"
 */

#ifndef ALLVARS_H
#define ALLVARS_H

#include "config-migrate.h"

#include <mpi.h>
#include <stdio.h>
#include <stdint.h>

#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_errno.h>

#include <signal.h>
#define BREAKPOINT raise(SIGTRAP)
#ifdef _OPENMP
#include <omp.h>
#include <pthread.h>
#else
#ifndef __clang_analyzer__
#error no OMP
#endif
#define omp_get_max_threads()  (1)
#define omp_get_thread_num()  (0)
#endif

#include "cosmology.h"
#include "walltime.h"

#include "assert.h"
#include "peano.h"


#define NEAREST(x) (((x)>0.5*All.BoxSize)?((x)-All.BoxSize):(((x)<-0.5*All.BoxSize)?((x)+All.BoxSize):(x)))

#ifndef  GENERATIONS
#define  GENERATIONS     4	/*!< Number of star particles that may be created per gas particle */
#endif

#define  TIMEBINS         29

#define  TIMEBASE        (1<<TIMEBINS)	/*!< The simulated timespan is mapped onto the integer interval [0,TIMESPAN],
                                         *   where TIMESPAN needs to be a power of 2. Note that (1<<28) corresponds
                                         *   to 2^29
                                         */
#define MAXHSML 30000.0

#ifdef ONEDIM
#define DIMS 1
#else
#ifdef TWODIMS    /* will only be compiled in 2D case */
#define DIMS 2
#else
#define DIMS 3
#endif
#endif

#ifndef  TOPNODEFACTOR
#define  TOPNODEFACTOR       2.5
#endif

#define  NODELISTLENGTH      8

#ifndef  GAMMA
#define  GAMMA         (5.0/3.0)	/*!< adiabatic index of simulated gas */
#endif

#define  GAMMA_MINUS1  (GAMMA-1)

#define  HYDROGEN_MASSFRAC 0.76	/*!< mass fraction of hydrogen, relevant only for radiative cooling */

#define  METAL_YIELD       0.02	/*!< effective metal yield for star formation */

#define  MAX_REAL_NUMBER  1e37
#define  MIN_REAL_NUMBER  1e-37

#define  RNDTABLE 8192

/* ... often used physical constants (cgs units) */

#define  GRAVITY     6.672e-8
#define  SOLAR_MASS  1.989e33
#define  SOLAR_LUM   3.826e33
#define  RAD_CONST   7.565e-15
#define  AVOGADRO    6.0222e23
#define  BOLTZMANN   1.38066e-16
/*Stefan-Boltzmann constant in cgs units*/
#define  STEFAN_BOLTZMANN 5.670373e-5
#define  GAS_CONST   8.31425e7
#define  C           2.9979e10
#define  PLANCK      6.6262e-27
#define  CM_PER_MPC  3.085678e24
#define  PROTONMASS  1.6726e-24
#define  ELECTRONMASS 9.10953e-28
#define  THOMPSON     6.65245e-25
#define  ELECTRONCHARGE  4.8032e-10
#define  HUBBLE          3.2407789e-18	/* in h/sec */
#define  LYMAN_ALPHA      1215.6e-8	/* 1215.6 Angstroem */
#define  LYMAN_ALPHA_HeII  303.8e-8	/* 303.8 Angstroem */
#define  OSCILLATOR_STRENGTH       0.41615
#define  OSCILLATOR_STRENGTH_HeII  0.41615

#define  SEC_PER_MEGAYEAR   3.155e13
#define  SEC_PER_YEAR       3.155e7

/*Determines the maximum size of arrays related to the number of CR populations */
#ifndef NUMCRPOP   /*!< Number of CR populations pressent in parameter file */
#define NUMCRPOP 1
#endif



#ifndef ASMTH
/*! ASMTH gives the scale of the short-range/long-range force split in units of FFT-mesh cells */
#define ASMTH 1.25
#endif
#ifndef RCUT
/*! RCUT gives the maximum distance (in units of the scale used for the force split) out to which short-range
 * forces are evaluated in the short-range tree walk.
 */
#define RCUT  4.5
#endif

#define COND_TIMESTEP_PARAMETER 0.25
#define VISC_TIMESTEP_PARAMETER 0.25

#define MAXLEN_OUTPUTLIST 12000	/*!< maxmimum number of entries in output list */

#define DRIFT_TABLE_LENGTH  1000	/*!< length of the lookup table used to hold the drift and kick factors */


#define MAXITER 400

#define MINRESTFAC 0.05


typedef uint64_t MyIDType;

typedef LOW_PRECISION MyFloat;
typedef HIGH_PRECISION MyDouble;

struct unbind_data
{
    int index;
};


#define HAS(val, flag) ((flag & (val)) == (flag))
#ifdef BLACK_HOLES
enum BlackHoleFeedbackMethod {
     BH_FEEDBACK_TOPHAT   = 0x2,
     BH_FEEDBACK_SPLINE   = 0x4,
     BH_FEEDBACK_MASS     = 0x8,
     BH_FEEDBACK_VOLUME   = 0x10,
     BH_FEEDBACK_OPTTHIN  = 0x20,
};
#endif
/*
 * additional sfr criterion in addition to density threshold
 * All.StarformationCriterion */
enum StarformationCriterion {
    SFR_CRITERION_DENSITY = 1,
    SFR_CRITERION_MOLECULAR_H2 = 3, /* 2 + 1 */
    SFR_CRITERION_SELFGRAVITY = 5,  /* 4 + 1 */
    /* below are additional flags in SELFGRAVITY */
    SFR_CRITERION_CONVERGENT_FLOW = 13, /* 8 + 4 + 1 */
    SFR_CRITERION_CONTINUOUS_CUTOFF= 21, /* 16 + 4 + 1 */
};

/*
 * wind models SH03, VS08 and OFJT10
 * All.WindModel */
enum WindModel {
    WINDS_NONE = 0,
    WINDS_SUBGRID = 1,
    WINDS_DECOUPLE_SPH = 2,
    WINDS_USE_HALO = 4,
    WINDS_FIXED_EFFICIENCY = 8,
    WINDS_ISOTROPIC = 16,
};

enum DensityKernelType {
    DENSITY_KERNEL_CUBIC_SPLINE = 1,
    DENSITY_KERNEL_QUINTIC_SPLINE = 2,
    DENSITY_KERNEL_QUARTIC_SPLINE = 4,
};


static inline double DMAX(double a, double b) {
    if(a > b) return a;
    return b;
}
static inline double DMIN(double a, double b) {
    if(a < b) return a;
    return b;
}
static inline int IMAX(int a, int b) {
    if(a > b) return a;
    return b;
}
static inline int IMIN(int a, int b) {
    if(a < b) return a;
    return b;
}

#define FACT1 0.366025403785	/* FACT1 = 0.5 * (sqrt(3)-1) */
#define FACT2 0.86602540        /* FACT2 = 0.5 * sqrt(3) */



/*********************************************************/
/*  Global variables                                     */
/*********************************************************/


extern int FirstActiveParticle;
extern int *NextActiveParticle;

extern int TimeBinCount[TIMEBINS];
extern int TimeBinCountSph[TIMEBINS];
extern int TimeBinActive[TIMEBINS];

extern int FirstInTimeBin[TIMEBINS];
extern int LastInTimeBin[TIMEBINS];
extern int *NextInTimeBin;
extern int *PrevInTimeBin;

#ifdef BLACK_HOLES
extern double Local_BH_mass;
extern double Local_BH_dynamicalmass;
extern double Local_BH_Mdot;
extern double Local_BH_Medd;
#endif

extern int ThisTask;		/*!< the number of the local processor  */
extern int NTask;		/*!< number of processors */

extern int64_t GlobNumForceUpdate;

extern int MaxTopNodes;	        /*!< Maximum number of nodes in the top-level tree used for domain decomposition */

extern int RestartFlag;		/*!< taken from command line used to start code. 0 is normal start-up from
                              initial conditions, 1 is resuming a run from a set of restart files, while 2
                              marks a restart from a snapshot file. */
extern int RestartSnapNum;

extern int Flag_FullStep;	/*!< Flag used to signal that the current step involves all particles */

extern int GlobFlag;

extern int NumPart;		/*!< number of particles on the LOCAL processor */

/* Local number of particles; this shall be made into an array */
extern int NumPart;
extern int N_dm;
extern int N_sph;
extern int N_bh;
extern int N_star;

extern gsl_rng *random_generator;	/*!< the random number generator used */

extern double TimeOfLastTreeConstruction;	/*!< holds what it says */

extern double RndTable[RNDTABLE];


/* variables for input/output , usually only used on process 0 */


extern FILE *FdInfo,		/*!< file handle for info.txt log-file. */
       *FdEnergy,			/*!< file handle for energy.txt log-file. */
       *FdCPU;			/*!< file handle for cpu.txt log-file. */

#ifdef SFR
extern FILE *FdSfr;		/*!< file handle for sfr.txt log-file. */
#endif

#ifdef BLACK_HOLES
extern FILE *FdBlackHoles;	/*!< file handle for blackholes.txt log-file. */
#endif

/*! This structure contains data which is the SAME for all tasks (mostly code parameters read from the
 * parameter file).  Holding this data in a structure is convenient for writing/reading the restart file, and
 * it allows the introduction of new global variables in a simple way. The only thing to do is to introduce
 * them into this structure.
 */
extern struct global_data_all_processes
{
    int64_t TotNumPart;
    int64_t TotN_sph;
    int64_t TotN_dm;
    int64_t TotN_bh;
    int64_t TotN_star;
    int64_t TotN_neutrinos;

    int NumThreads;     /* number of threads used to simulate OpenMP tls */
    int MaxPart;			/*!< This gives the maxmimum number of particles that can be stored on one
                              processor. */
    int MaxPartSph;		/*!< This gives the maxmimum number of SPH particles that can be stored on one
                          processor. */
    int MaxPartBh;		/*!< This gives the maxmimum number of BH particles that can be stored on one
                          processor. */

    int DoDynamicUpdate;

    int NumPartPerFile;   /* Number of particles per physical file */
    int NumWriters;  /*Number of concurrent writers */
    int EnableAggregatedIO;  /* Enable aggregated IO policy for small files.*/

    double BufferSize;		/*!< size of communication buffer in MB */

    double PartAllocFactor;	/*!< in order to maintain work-load balance, the particle load will usually
                              NOT be balanced.  Each processor allocates memory for PartAllocFactor times
                              the average number of particles to allow for that */

    double TreeAllocFactor;	/*!< Each processor allocates a number of nodes which is TreeAllocFactor times
                              the maximum(!) number of particles.  Note: A typical local tree for N
                              particles needs usually about ~0.65*N nodes. */

    double TopNodeAllocFactor;	/*!< Each processor allocates a number of nodes which is TreeAllocFactor times
                                  the maximum(!) number of particles.  Note: A typical local tree for N
                                  particles needs usually about ~0.65*N nodes. */

    /* some SPH parameters */

    int DesNumNgb;		/*!< Desired number of SPH neighbours */
    double DensityResolutionEta;		/*!< SPH resolution eta. See Price 2011. eq 12*/
    double MaxNumNgbDeviation;	/*!< Maximum allowed deviation neighbour number */
    double ArtBulkViscConst;	/*!< Sets the parameter \f$\alpha\f$ of the artificial viscosity */

    double InitGasTemp;		/*!< may be used to set the temperature in the IC's */
    double MinGasTemp;		/*!< may be used to set a floor for the gas temperature */
    double MinEgySpec;		/*!< the minimum allowed temperature expressed as energy per unit mass */

    /* some force counters  */

    int64_t TotNumOfForces;	/*!< counts total number of force computations  */

    int64_t NumForcesSinceLastDomainDecomp;	/*!< count particle updates since last domain decomposition */

    /* system of units  */

    double UnitTime_in_s,		/*!< factor to convert internal time unit to seconds/h */
           UnitMass_in_g,		/*!< factor to convert internal mass unit to grams/h */
           UnitVelocity_in_cm_per_s,	/*!< factor to convert intqernal velocity unit to cm/sec */
           UnitLength_in_cm,		/*!< factor to convert internal length unit to cm/h */
           UnitPressure_in_cgs,	/*!< factor to convert internal pressure unit to cgs units (little 'h' still
                                  around!) */
           UnitDensity_in_cgs,		/*!< factor to convert internal length unit to g/cm^3*h^2 */
           UnitCoolingRate_in_cgs,	/*!< factor to convert internal cooling rate to cgs units */
           UnitEnergy_in_cgs,		/*!< factor to convert internal energy to cgs units */
           UnitTime_in_Megayears,	/*!< factor to convert internal time to megayears/h */
           GravityConstantInternal,	/*!< If set to zero in the parameterfile, the internal value of the
                                      gravitational constant is set to the Newtonian value based on the system of
                                      units specified. Otherwise the value provided is taken as internal gravity
                                      constant G. */
           G;				/*!< Gravity-constant in internal units */
    double UnitDensity_in_Gev_per_cm3; /*!< factor to convert internal density unit to GeV/c^2 / cm^3 */
    /* Cosmology */
    Cosmology CP;

    double Hubble; /*!< Hubble-constant in internal units */

    double BoxSize;   /* Boxsize in case periodic boundary conditions are used */

    /* Code options */
    int DomainOverDecompositionFactor; /* Number of sub-domains per processor. */

    int TypeOfOpeningCriterion;	/*!< determines tree cell-opening criterion: 0 for Barnes-Hut, 1 for relative
                                  criterion */
    int TypeOfTimestepCriterion;	/*!< gives type of timestep criterion (only 0 supported right now - unlike
                                      gadget-1.1) */
    int CoolingOn;		/*!< flags that cooling is enabled */
    double UVRedshiftThreshold;		/* Initial redshift of UV background. */
    int HydroOn;		/*!< flags that hydro force is enabled */
    int TreeGravOn;          /*!< flags that tree gravity force is enabled*/
    int BlackHoleOn;		/*!< flags that black holes are enabled */
    int StarformationOn;		/*!< flags that star formation is enabled */
    enum StarformationCriterion StarformationCriterion;		/*!< flags that star formation is enabled */
    enum WindModel WindModel;		/*!< flags that star formation is enabled */

    int MakeGlassFile; /*!< flags that gravity is reversed and we are making a glass file*/
    int NoTreeType; /*!< flags a particle species to exclude from tree forces*/
    int FastParticleType; /*!< flags a particle species to exclude timestep calculations.*/
    /* parameters determining output frequency */

    int SnapshotFileCount;	/*!< number of snapshot that is written next */
    double TimeOfFirstSnapshot,	/*!< simulation time of first snapshot files */
           CpuTimeBetRestartFile,	/*!< cpu-time between regularly generated restart files */
           TimeLastRestartFile;  	/*!< cpu-time when last restart-file was written */

    int NumCurrentTiStep;		/*!< counts the number of system steps taken up to this point */

    /* Current time of the simulation, global step, and end of simulation */

    double Time,			/*!< current time of the simulation */
           TimeBegin,			/*!< time of initial conditions of the simulation */
           TimeStep,			/*!< difference between current times of previous and current timestep */
           TimeMax;			/*!< marks the point of time until the simulation is to be evolved */

    struct {
        double a;
        double a3inv;
        double a2inv;
        double fac_egy;
        double hubble;
        double hubble_a2;
        double D1;
    } cf;

    /* variables for organizing discrete timeline */

    double Timebase_interval;	/*!< factor to convert from floating point time interval to integer timeline */
    int Ti_Current;		/*!< current time on integer timeline */
    int Ti_nextoutput;		/*!< next output time on integer timeline */

    int Nmesh;

    int PM_Ti_endstep, PM_Ti_begstep;
    double Asmth[2], Rcut[2];
    double Corner[2][3], UpperCorner[2][3], Xmintot[2][3], Xmaxtot[2][3];
    double TotalMeshSize[2];

    int Ti_nextlineofsight;

    /* variables that keep track of cumulative CPU consumption */

    double TimeLimitCPU;
    struct ClockTable CT;

    /* tree code opening criterion */

    double ErrTolTheta;		/*!< BH tree opening angle */
    double ErrTolForceAcc;	/*!< parameter for relative opening criterion in tree walk */


    /* adjusts accuracy of time-integration */

    double ErrTolIntAccuracy;	/*!< accuracy tolerance parameter \f$ \eta \f$ for timestep criterion. The
                                  timesteps is \f$ \Delta t = \sqrt{\frac{2 \eta eps}{a}} \f$ */

    double MinSizeTimestep,	/*!< minimum allowed timestep. Normally, the simulation terminates if the
                              timestep determined by the timestep criteria falls below this limit. */
           MaxSizeTimestep;		/*!< maximum allowed timestep */

    double MaxRMSDisplacementFac;	/*!< this determines a global timestep criterion for cosmological simulations
                                      in comoving coordinates.  To this end, the code computes the rms velocity
                                      of all particles, and limits the timestep such that the rms displacement
                                      is a fraction of the mean particle separation (determined from the
                                      particle mass and the cosmological parameters). This parameter specifies
                                      this fraction. */

    double MaxGasVel; /* Limit on Gas velocity */
    int MaxMemSizePerCore;

    double CourantFac;		/*!< SPH-Courant factor */


    /* frequency of tree reconstruction/domain decomposition */


    double TreeDomainUpdateFrequency;	/*!< controls frequency of domain decompositions  */


    /* gravitational and hydrodynamical softening lengths (given in terms of an `equivalent' Plummer softening
     * length)
     *
     * five groups of particles are supported 0=gas,1=halo,2=disk,3=bulge,4=stars
     */
    double MinGasHsmlFractional,	/*!< minimum allowed SPH smoothing length in units of SPH gravitational
                                      softening length */
           MinGasHsml;			/*!< minimum allowed SPH smoothing length */


    enum DensityKernelType DensityKernelType;  /* 0 for Cubic Spline,  (recmd NumNgb = 33)
                               1 for Quintic spline (recmd  NumNgb = 97)
                             */
    double DensityContrastLimit; /* limit of density contrast ratio for hydro force calculation */
    double HydroCostFactor; /* cost factor for hydro in load balancing. */
    double SofteningGas,		/*!< for type 0 */
           SofteningHalo,		/*!< for type 1 */
           SofteningDisk,		/*!< for type 2 */
           SofteningBulge,		/*!< for type 3 */
           SofteningStars,		/*!< for type 4 */
           SofteningBndry;		/*!< for type 5 */

    double SofteningGasMaxPhys,	/*!< for type 0 */
           SofteningHaloMaxPhys,	/*!< for type 1 */
           SofteningDiskMaxPhys,	/*!< for type 2 */
           SofteningBulgeMaxPhys,	/*!< for type 3 */
           SofteningStarsMaxPhys,	/*!< for type 4 */
           SofteningBndryMaxPhys;	/*!< for type 5 */

    double SofteningTable[6];	/*!< current (comoving) gravitational softening lengths for each particle type */
    double ForceSoftening[6];	/*!< the same, but multiplied by a factor 2.8 - at that scale the force is Newtonian */


    /*! If particle masses are all equal for one type, the corresponding entry in MassTable is set to this
     *  value, * allowing the size of the snapshot files to be reduced
     */
    double MassTable[6];


    /* some filenames */
    char InitCondFile[100],
         TreeCoolFile[100],
         MetalCoolFile[100],
         OutputDir[100],
         SnapshotFileBase[100],
         EnergyFile[100],
         CpuFile[100],
         InfoFile[100], ResubmitCommand[100], OutputList[100];

    char UVFluctuationFile[100];

    /*! table with desired output times */
    double OutputListTimes[8192];
    int OutputListLength;		/*!< number of times stored in table of desired output times */



#if defined(ADAPTIVE_GRAVSOFT_FORGAS) && !defined(ADAPTIVE_GRAVSOFT_FORGAS_HSML)
    double ReferenceGasMass;
#endif

#ifdef SFR		/* star formation and feedback sector */
    double CritOverDensity;
    double CritPhysDensity;
    double OverDensThresh;
    double PhysDensThresh;
    double EgySpecSN;
    double FactorSN;
    double EgySpecCold;
    double FactorEVP;
    double FeedbackEnergy;
    double TempSupernova;
    double TempClouds;
    double MaxSfrTimescale;
    double WindFreeTravelLength;
    double WindFreeTravelDensFac;
    double FactorForSofterEQS;
    double QuickLymanAlphaProbability;

    /* used in VS08 and SH03*/
    double WindEfficiency;
    double WindSpeed;
    double WindEnergyFraction;
    /* used in OFJT10*/
    double WindSigma0;
    double WindSpeedFactor;
#endif

#ifdef BLACK_HOLES
    double BlackHoleAccretionFactor;	/*!< Fraction of BH bondi accretion rate */
    double BlackHoleFeedbackFactor;	/*!< Fraction of the black luminosity feed into thermal feedback */
    enum BlackHoleFeedbackMethod BlackHoleFeedbackMethod;	/*!< method of the feedback*/
    double BlackHoleFeedbackRadius;	/*!< Radius the thermal feedback is fed comoving*/
    double BlackHoleFeedbackRadiusMaxPhys;	/*!< Radius the thermal cap */
    double SeedBlackHoleMass;	/*!< Seed black hole mass */
    double BlackHoleNgbFactor;	/*!< Factor by which the normal SPH neighbour should be increased/decreased */
    double BlackHoleMaxAccretionRadius;
    double BlackHoleEddingtonFactor;	/*! Factor above Eddington */
    int BlackHoleSoundSpeedFromPressure; /* 0 from Entropy, 1 from Pressure; */
#endif

    int SnapshotWithFOF; /*Flag that doing FOF for snapshot outputs is on*/
    double MinFoFMassForNewSeed;	/* Halo mass required before new seed is put in */
    double FOFHaloLinkingLength;    
    double FOFHaloComovingLinkingLength; /* in code units */
    int FOFHaloMinLength;
    double TimeNextSeedingCheck;  /*Time for the next seed check.*/
    double TimeBetweenSeedingSearch; /*Factor to multiply TimeBegin by to find the next seeding check.*/

}
All;
#ifdef _OPENMP
extern size_t BlockedParticleDrifts;
extern size_t TotalParticleDrifts;
extern size_t BlockedNodeDrifts;
extern size_t TotalNodeDrifts;
#endif
struct bh_particle_data {
    int ReverseLink; /* used at GC for reverse link to P */
    MyIDType ID; /* for data consistency check, same as particle ID */
    int CountProgs;

    MyFloat Mass;
    MyFloat Mdot;
    MyFloat FeedbackWeightSum;
    MyFloat Density;
    MyFloat Entropy;
    MyFloat Pressure;
    MyFloat SurroundingGasVel[3];

    MyFloat accreted_Mass;
    MyFloat accreted_BHMass;
    MyFloat accreted_momentum[3];

    double  MinPotPos[3];
    MyFloat MinPotVel[3];
    MyFloat MinPot;

    short int TimeBinLimit;
} * BhP;

/*! This structure holds all the information that is
 * stored for each particle of the simulation.
 */
extern struct particle_data
{
#ifdef OPENMP_USE_SPINLOCK
    pthread_spinlock_t SpinLock;
#endif

    float GravCost;		/*!< weight factor used for balancing the work-load */

    int Ti_begstep;		/*!< marks start of current timestep of particle on integer timeline */
    int Ti_current;		/*!< current time of the particle */


    double Pos[3];   /*!< particle position at its current time */
    float Mass;     /*!< particle mass */
    struct {
        unsigned int Evaluated :1;
        unsigned int DensityIterationDone :1;
        unsigned int OnAnotherDomain     :1;
        unsigned int WillExport    :1; /* used in domain */
        unsigned int Type        :4;		/*!< flags particle type.  0=gas, 1=halo, 2=disk, 3=bulge, 4=stars, 5=bndry */
        /* first byte ends */
        signed char TimeBin;
        /* second byte ends */
        unsigned char Generation; /* How many particles it has spawned*/
#ifdef WINDS
        unsigned int IsNewParticle:1; /* whether it is created this step */
#endif
#ifdef BLACK_HOLES
        unsigned int Swallowed : 1; /* whether it is being swallowed */
#endif
    };

    unsigned int PI; /* particle property index; used by BH. points to the BH property in BhP array.*/
    MyIDType ID;

    MyFloat Vel[3];   /* particle velocity at its current time */
    MyFloat GravAccel[3];  /* particle acceleration due to short-range gravity */

    MyFloat GravPM[3];		/* particle acceleration due to long-range PM gravity force */
    MyFloat OldAcc;			/* magnitude of old gravitational force. Used in relative opening
                              criterion, only used by gravtree cross time steps */

    MyFloat Potential;		/* gravitational potential. This is the total potential after gravtree is called. */
    MyFloat PM_Potential;  /* Only used by PM. useless after pm */

    MyFloat StarFormationTime;		/*!< formation time of star particle: needed to tell when wind is active. */

#ifdef METALS
    MyFloat Metallicity;		/*!< metallicity of gas or star particle */
#endif				/* closes METALS */

    MyFloat Hsml;

#ifdef BLACK_HOLES
    /* SwallowID is not reset in blackhole.c thus cannot be in a union */
    MyIDType SwallowID; /* who will swallow this particle, used only in blackhole.c */
#endif

    union {
        /* the following variables are transients.
         * FIXME: move them into the corresponding modules! Is it possible? */

        MyFloat NumNgb; /* Number of neighbours; only used in density.c */

        int RegionInd; /* which region the particle belongs to; only by petapm.c */

        /* The peano key is a hash of the position used in the domain decomposition.
         * It is slow to generate so we store it here.*/
        peanokey Key; /* only by domain.c */
        struct {
            /* used by fof.c which calls domain_exchange that doesn't uses peanokey */
            int64_t GrNr; 
            int origintask;
            int targettask;
        };
    };

}
*P;				/*!< holds particle data on local processor */


/* the following structure holds data that is stored for each SPH particle in addition to the collisionless
 * variables.
 */
extern struct sph_particle_data
{
#ifdef DENSITY_INDEPENDENT_SPH
    MyFloat EgyWtDensity;           /*!< 'effective' rho to use in hydro equations */
    MyFloat EntVarPred;             /*!< predicted entropy variable */
    MyFloat DhsmlEgyDensityFactor;  /*!< correction factor for density-independent entropy formulation */
#define EOMDensity EgyWtDensity
#else
#define EOMDensity Density
#endif

    MyFloat Entropy;		/*!< current value of entropy (actually entropic function) of particle */
    MyFloat Pressure;		/*!< current pressure */
    MyFloat VelPred[3];		/*!< predicted SPH particle velocity at the current time */
    MyFloat MaxSignalVel;           /*!< maximum signal velocity */
#ifdef VOLUME_CORRECTION
    MyFloat DensityOld;
    MyFloat DensityStd;
#endif

    MyFloat       Density;		/*!< current baryonic mass density of particle */
    MyFloat       DtEntropy;		/*!< rate of change of entropy */
    MyFloat       HydroAccel[3];	/*!< acceleration due to hydrodynamical force */
    MyFloat       DhsmlDensityFactor;	/*!< correction factor needed in entropy formulation of SPH */
    MyFloat       DivVel;		/*!< local velocity divergence */
    MyFloat       CurlVel;     	        /*!< local velocity curl */
    MyFloat       Rot[3];		/*!< local velocity curl */
    MyFloat Ne;  /*!< electron fraction, expressed as local electron number
                   density normalized to the hydrogen number density. Gives
                   indirectly ionization state and mean molecular weight. */

#ifdef BLACK_HOLES
    MyFloat       Injected_BH_Energy;
#endif

#ifdef SFR
    MyFloat Sfr;
#endif
#ifdef WINDS
    MyFloat DelayTime;		/*!< SH03: remaining maximum decoupling time of wind particle */
                            /*!< VS08: remaining waiting for wind particle to be eligible to form winds again */
#endif

#ifdef SPH_GRAD_RHO
    MyFloat GradRho[3];
#endif
} *SphP;				/*!< holds SPH particle data on local processor */

#define SPHP(i) SphP[i]
#define BHP(i) BhP[P[i].PI]

/* global state of system
*/
extern struct state_of_system
{
    double Mass;
    double EnergyKin;
    double EnergyPot;
    double EnergyInt;
    double EnergyTot;

    double Momentum[4];
    double AngMomentum[4];
    double CenterOfMass[4];
    double MassComp[6];
    /* Only Gas is used */
    double TemperatureComp[6];

    double EnergyKinComp[6];
    double EnergyPotComp[6];
    double EnergyIntComp[6];
    double EnergyTotComp[6];
    double MomentumComp[6][4];
    double AngMomentumComp[6][4];
    double CenterOfMassComp[6][4];
}
SysState, SysStateAtStart, SysStateAtEnd;

#define MPI_UINT64 MPI_UNSIGNED_LONG
#define MPI_INT64 MPI_LONG

static inline double
dotproduct(double v1[3], double v2[3])
{
    double r =0;
    int d;
    for(d = 0; d < 3; d ++) {
        r += v1[d] * v2[d];
    }
    return r;
}

static inline void crossproduct(double v1[3], double v2[3], double out[3])
{
    static int D2[3] = {1, 2, 0};
    static int D3[3] = {2, 0, 1};

    int d1, d2, d3;

    for(d1 = 0; d1 < 3; d1++)
    {
        d2 = D2[d1];
        d3 = D3[d1];

        out[d1] = (v1[d2] * v2[d3] -  v2[d2] * v1[d3]);
    }
}

#endif
