#ifndef AFA_SPECTRA_H__
#define AFA_SPECTRA_H__

#include <stddef.h>

#include "AFATypes.h"
#include "AFASpectraCommon.h"

/** @name ACCESSORS
*/
//@{
void
AFASpectraPixelStartEndGet(
    uint32_t *pStart,
    uint32_t *pEnd );

#if 0
// returns true if marked as empty spectrum (specObjID is set to zero).
bool_t
AFASpectraIsEmpty(
	AFASpectra_SW *sp);
#endif

// compare spectra and return accumulated quadratic error of all compared samples (euclidean style).
float
AFASpectraCompare(
	AFASpectra_SW *sp1,
	AFASpectra_SW *sp2);


//@}

/** @name MODIFIERS
*/
//@{

// clear data/reset spectrum
void
AFASpectraClear(
	AFASpectra_SW *sp );

void
AFASpectraSet(
	AFASpectra_SW *dst,
	AFASpectra_SW *src );

// set sine curve with a given frequency, phase and amplitude.
// Good to model test spectra
void
AFASpectraSetSine(
	AFASpectra_SW *sp,
    float32_t _frequency,
    float32_t _phase,
    float32_t _amplitude,
    float32_t _noize );

// fill spectrum with noise
void
AFASpectraRandomize(
	AFASpectra_SW *sp,
    float32_t _minrange,
    float32_t _maxrange);

// calculate extrema
void
AFASpectraCalcMinMax(
	AFASpectra_SW *sp );

// calculates the surface of the spectrum
void
AFASpectraCalculateFlux(
	AFASpectra_SW *sp );

// normalize by flux
void
AFASpectraNormalizeByFlux(
	AFASpectra_SW *sp );


// adapt spectrum towards another spectrum by a given factor
// src spectrum to adapt to
// _adaptionRate [0..1]
void
AFASpectraAdapt(
	AFASpectra_SW *dst,
	AFASpectra_SW *src,
    float32_t _adaptionRate );

//@}


/** @name HELPER FUNCTIONS
*/
//@{

// if we use BOSS and SDSS spectra combined calculate offset for SDSS spectra.
// BOSS spectra start at 3650A, SDSS spectra at 3800A -> thus use offset ~13 so both types operate in equal wavelengths
int
AFASpectraGetSDSSSpectraOffsetStart();

int
AFASpectraGetSDSSSpectraOffsetEnd();

// set to true if we should use the entire BOSS wavelength range during clustering or false if only default SDSS wavelength range should be used.
void
AFASpectraSetOperationRange(
    bool_t _BOSSWavelengthRange );

//@}

#endif
