#include "AFAConfig.h"
#include "AFADefines.h"
#include "AFATypes.h"
#include "AFASpectra.h"
#include "AFAProcessing.h"
#include "AFATime.h"

#ifdef AFA_RUN_ON_XILINX_SDK
// BSP Includes
#include "xparameters.h"
#include "xil_cache.h"
#include "xtmrctr.h"

// Driver includes
#include "DriverInterruptController.h"
#include "DriverTimer.h"
#include "DriverAFAProcessing.h"
#endif

#include "BoardIO.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define JSCDBG_ITER_SPECIAL
// #define JSCDBG_PRINTOUT_GOLDEN_SAMPLE_ONLY
// #define AFA_TEST_SHORT_ITERATION_0000

extern AFAProcessingParamSW_t       AFAPP_sw;
extern int m_mti; 

// philipp short
extern const unsigned long testAFAshort_size;
extern unsigned char testAFAshort_data[];
extern const unsigned long testAFAshortIndex0000_size;
extern const unsigned char testAFAshortIndex0000_data[];
extern const unsigned long testAFAshortIndex0001_size;
extern const unsigned char testAFAshortIndex0001_data[];
extern const unsigned long testAFAshortIndex0200_size;
extern const unsigned char testAFAshortIndex0200_data[];


// current learning step
sint32_t currentStep;

sint16_t golden_data[ 12 ][ 12 ]=
{
#ifdef JSCDBG_ITER_SPECIAL
// params.numSteps = 1;
    {   8,    2,   12,   14,   17,   18,   52,   49,   51,   48,   47,   40, },
    {  90,    3,    7,   15,   11,   16,   53,   55,   54,   46,   38,   45, },
    {  92,   89,    5,   10,    9,   13,   -1,   56,   50,   41,   44,   35, },
    {  91,   87,    1,    4,   93,   -1,   -1,   -1,   -1,   43,   42,   36, },
    {  95,   94,   96,    0,    6,   -1,   -1,   -1,   -1,   -1,   37,   39, },
    {  97,   98,   99,   86,   -1,   -1,   -1,   -1,   -1,   -1,   32,   34, },
    {  84,   88,   85,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   20,   33, },
    {  83,   80,   -1,   -1,   -1,   -1,   -1,   57,   -1,   -1,   23,   30, },
    {  81,   77,   -1,   -1,   -1,   -1,   61,   58,   -1,   22,   29,   31, },
    {  78,   76,   -1,   71,   67,   66,   62,   59,   60,   24,   25,   28, },
    {  82,   74,   75,   70,   68,   65,   64,   63,   -1,   -1,   27,   21, },
    {  79,   73,   72,   69,   -1,   -1,   -1,   -1,   -1,   -1,   19,   26, }
#else

///////////////////////////////////////////////////////////
#define JSCDBG_ACCEPT_LITTLE_INACCURACIES
///////////////////////////////////////////////////////////

#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
// params.numSteps = 200;
    {  10,   11,   13,   15,   18,   19,   25,   27,   29,   30,   33,   34, },
    {   8,    9,   12,   14,   17,   20,   24,   26,   31,   32,   35,   36, },
    {   7,    4,   -1,   -1,   16,   21,   23,   28,   -1,   -1,   37,   38, },
    {   6,    2,    5,   -1,   -1,   22,   -1,   -1,   -1,   39,   40,   41, },
    {   0,    1,    3,   -1,   -1,   -1,   -1,   -1,   -1,   43,   42,   44, },
    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,   46,   45, },
    {  97,   95,   98,   99,   -1,   -1,   -1,   -1,   -1,   51,   49,   48, },
    {  93,   94,   92,   96,   -1,   -1,   -1,   -1,   -1,   50,   53,   52, },
    {  90,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   54,   55, },
    {  88,   89,   -1,   81,   -1,   76,   72,   70,   65,   60,   57,   56, },
    {  87,   82,   84,   79,   78,   74,   73,   69,   66,   62,   59,   58, },
    {  86,   85,   83,   80,   77,   75,   71,   68,   67,   64,   63,   61, }
#else
// params.numSteps = 200;
    {  10,   11,   13,   15,   18,   19,   25,   27,   29,   30,   33,   34, },
    {   8,    9,   12,   14,   17,   20,   24,   26,   31,   32,   35,   36, },
    {   7,    4,   -1,   -1,   16,   21,   23,   28,   -1,   -1,   37,   38, },
    {   6,    2,    5,   -1,   -1,   22,   -1,   -1,   -1,   39,   40,   41, },
    {   0,    1,    3,   -1,   -1,   -1,   -1,   -1,   -1,   43,   42,   44, },
    {  -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   47,   46,   45, },
    {  97,   95,   98,   99,   -1,   -1,   -1,   -1,   -1,   51,   49,   48, },
    {  93,   94,   92,   96,   -1,   -1,   -1,   -1,   -1,   50,   53,   52, },
    {  90,   91,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   54,   55, },
    {  88,   89,   -1,   81,   -1,   76,   72,   70,   65,   60,   57,   56, },
    {  87,   82,   84,   79,   78,   74,   73,   69,   66,   62,   59,   58, },
    {  86,   85,   83,   80,   77,   75,   71,   68,   67,   64,   63,   61, }
#endif
#endif
};

#ifdef AFA_RUN_ON_XILINX_SDK
void
TestClockFunction()
{
    clock_t timeIterationStart;
    clock_t timeIterationEnd;
    sint32_t numSecs = 3;

    timeIterationStart = clock();
    if ( timeIterationStart )
    {
		while( numSecs >= 0 )
		{
			timeIterationEnd = clock();
			if (((( double )( timeIterationEnd - timeIterationStart )) / (( double ) CLOCKS_PER_SEC ) )>= 1.0 )
			{
				printf( "t=%d\n", numSecs );
				timeIterationStart = timeIterationEnd;
				numSecs--;
			}
		}
    }
    else
    {
    	printf( "timer not working (not initialized?)\n" );
    }
}

void AssertCallback(
	const char8 *File,
	s32 Line )
{
	printf( "ASSERTION: File=%s, Line=%ld\n", File, Line );
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_RED );		// ERROR
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLUE );		// ASSERT
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLUE );		// ASSERT
	LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLUE );		// ASSERT
}
#endif


void generateSineTestSpectra( uint32_t numTestSpectra, AFASpectra_SW *outSpectraArray )
{
    uint32_t i;
    float freqMin = 0.002f;
    float freqMax = 0.05f;
    float freqStepSize = (freqMax-freqMin)/(float)numTestSpectra;
    float freq = freqMin;
    AFASpectra_SW *sp;

    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_YELLOW );		// sine state
    for (i=0;i<numTestSpectra;i++)
    {
    	LEDBinaryShow( i );
        sp =  &outSpectraArray[i];
        AFASpectraSetSine( sp, freq, 0.0f, 1.0f, 0.0f );
        freq += freqStepSize;
    }
}



// translates sw spectra to hw spectra. Can be used for input spectra as well as network spectra.
// spectraArraySw incoming sw spectra
// outHWAddr allocated hardware memory where we write AFA_SPECTRA_INDEX_SIZE_IN_BYTES*numSpectra bytes
void
swSpectraToHwSpectra(
    AFASpectra_SW *spectraArraySw,
    uint32_t *outHWAddr,
    uint32_t numSpectra )
{
   uint64_t spectraAdress;
   uint32_t i,j;
   AFASpectra_SW *sp;

   for ( i = 0; i < numSpectra; i++ )
   {
      sp = &spectraArraySw[ i ];
      spectraAdress =  i * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;

      for ( j = 0; j < AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; j++ )
      {
         // cast to float ptr to write float values
         float32_t *tmpValFloatPtr = (( float32_t * ) &outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_AMPLITUDE + j ]);
         ( *tmpValFloatPtr ) = sp->m_Amplitude[ j + AFA_PROCESS_PIXEL_START ];
      }

      outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_INDEX            ] = sp->m_Index;
outHWAddr[ spectraAdress + AFA_DEBUG_MAGIC ] = 0x12345678;
outHWAddr[ spectraAdress + AFA_DEBUG_JSC ] = sp->m_pad2;
      outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] = ( uint32_t )( sp->m_SpecObjID & 0x0ffffffff );
      outHWAddr[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = ( uint32_t )( sp->m_SpecObjID >> 32 );
   }
}

// translates hw spectra back to sw spectra. Can be used for inputs spectra as well as network spectra.
// spectraArrayHw incoming hw spectra with AFA_SPECTRA_INDEX_SIZE_IN_BYTES*numSpectra data
// outSwArray allocated software memory where we write spectra to
void
hwSpectraToSwSpectra(
   uint32_t *spectraArrayHw,
   AFASpectra_SW *outSwArray,
   uint32_t numSpectra )
{
   uint64_t spectraAdress;
   uint32_t i,j;
   AFASpectra_SW *outSp;
   float32_t *tmpValFloatPtr;


   for ( i = 0; i < numSpectra; i++ )
   {
      outSp = &outSwArray[ i ];

      spectraAdress =  i * AFA_SPECTRA_INDEX_SIZE_IN_UINT32;
      tmpValFloatPtr = (( float32_t * ) &spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_AMPLITUDE ]);

      for ( j = 0; j < AFA_SPECTRA_NUM_SAMPLES_PROCESS_HW; j++ )
      {
         // cast to float ptr to read float values
         outSp->m_Amplitude[ j + AFA_PROCESS_PIXEL_START ] = tmpValFloatPtr[ j ];
      }

      outSp->m_Index = spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_INDEX ];
      outSp->m_SpecObjID = ( uint64_t )spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] |
                          (( uint64_t )spectraArrayHw[ spectraAdress + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] << 32);
   }
}

bool_t readSpectraFileToHWSpectra(
    const char *filename,
    uint32_t numSpectra,
    uint32_t *outHWAddr )
{
    uint32_t i;
    uint64_t retVal;
    FILE *f = 0;
    AFASpectra_SW tempSp;

    f = fopen( filename, "rb" );

    if (f == NULL )
    {
        return FALSE;
    }

    for ( i = 0; i < numSpectra; i++ )
    {
        retVal = fread( &tempSp, sizeof(AFASpectra_SW), 1, f );

        if ( retVal != 1 )
        {
            fclose(f);
            return FALSE;
        }
    }

    fclose( f );
    return TRUE;
}

#if 0
typedef struct
{
    const unsigned char *spectraDataPtr;
    const unsigned long *spectraDataSize;
} SpectraArrayChunk_t;

SpectraArrayChunk_t spectraArrayChunks[] =
{
    { afaTestDataSpectra_data00_data, &afaTestDataSpectra_data00_size },
    { afaTestDataSpectra_data01_data, &afaTestDataSpectra_data01_size },
    { afaTestDataSpectra_data02_data, &afaTestDataSpectra_data02_size },
    { afaTestDataSpectra_data03_data, &afaTestDataSpectra_data03_size },
    { afaTestDataSpectra_data04_data, &afaTestDataSpectra_data04_size },
    { afaTestDataSpectra_data05_data, &afaTestDataSpectra_data05_size },
    { afaTestDataSpectra_data06_data, &afaTestDataSpectra_data06_size },
    { afaTestDataSpectra_data07_data, &afaTestDataSpectra_data07_size },
    { afaTestDataSpectra_data08_data, &afaTestDataSpectra_data08_size },
    { afaTestDataSpectra_data09_data, &afaTestDataSpectra_data09_size },
    { afaTestDataSpectra_data10_data, &afaTestDataSpectra_data10_size },
    { afaTestDataSpectra_data11_data, &afaTestDataSpectra_data11_size },
    { afaTestDataSpectra_data12_data, &afaTestDataSpectra_data12_size },
    { afaTestDataSpectra_data13_data, &afaTestDataSpectra_data13_size }
};

enum
{
    AFA_TEST_DATA_SPECTRA_NUM_ARRAY_CHUNKS = sizeof( spectraArrayChunks ) / sizeof( SpectraArrayChunk_t ),
};
static uint32_t readFromArrayOffset = 0;
static uint32_t readFromArrayIndex = 0;
uint64_t readFromArraySize = 0;

uint64_t getSpectraArraySize();
void ReadFromArrayInit();
uint32_t ReadFromArrayDataRead( void *dest, uint64_t size );

void
ReadFromArrayInit(
    )
{
    readFromArrayOffset = 0;
    readFromArrayIndex = 0;
    readFromArraySize = getSpectraArraySize();
}

uint32_t
ReadFromArrayDataRead(
    void *dest,
    uint64_t size )
{
    uint64_t chunkSize = 0;
    uint32_t diff;
    uint8_t *p;

    chunkSize = *( spectraArrayChunks[ readFromArrayIndex ].spectraDataSize );
    if ( readFromArrayOffset + size > chunkSize )
    {
        if ( readFromArrayIndex < AFA_TEST_DATA_SPECTRA_NUM_ARRAY_CHUNKS - 1 )
        {
            diff = chunkSize - readFromArrayOffset;
            memcpy( dest, &( spectraArrayChunks[ readFromArrayIndex ].spectraDataPtr[ readFromArrayOffset ]), diff );
            p = ( uint8_t * ) dest;
            readFromArrayIndex++;
            readFromArrayOffset = size - diff;
            memcpy( &p[ diff ], &( spectraArrayChunks[ readFromArrayIndex ].spectraDataPtr[ 0 ]), readFromArrayOffset );
        }
        else
        {
            return 0;
        }
    }
    else
    {
        memcpy( dest, &( spectraArrayChunks[ readFromArrayIndex ].spectraDataPtr[ readFromArrayOffset ]), size );
        readFromArrayOffset += size;
    }
    return 1;
}

bool_t readSpectraArrayToHWSpectra(
    uint32_t numSpectra,
    uint32_t *outHWAddr )
{
    uint32_t i;
    uint64_t retVal, offset = 0;
    AFASpectra_SW tempSp;

    ReadFromArrayInit();
    for ( i = 0; i < numSpectra; i++ )
    {

        retVal = ReadFromArrayDataRead( &tempSp, sizeof( AFASpectra_SW ));

        if ( retVal != 1 )
        {
            return FALSE;
        }
    }

    return TRUE;
}
uint64_t
getSpectraArraySize()
{
    uint32_t i;
    uint64_t size = 0;
    for ( i = 0; i < AFA_TEST_DATA_SPECTRA_NUM_ARRAY_CHUNKS; ++i )
    {
        size += *( spectraArrayChunks[ i ].spectraDataSize );
    }
    return size;
}
#else
	// short Philipp
bool_t readSpectraArrayToHWSpectra(
    uint32_t numSpectra,
    uint32_t *outHWAddr )
{
    return TRUE;
}
uint64_t
getSpectraArraySize()
{
    return testAFAshort_size;
}
#endif

uint32_t
AFAGetSpectraIndexNew(
    uint32_t xp,
    uint32_t yp )
{
    uint32_t adr = xp + yp * AFAPP_sw.m_gridSize;

    if ( adr >= AFAPP_sw.m_gridSizeSqr )
        return ( uint32_t ) ( -1 );

    return ( uint32_t )AFAPP_sw.spectraDataWorkingSetHW[ AFA_SPECTRA_INDEX_INDEX + adr * AFA_SPECTRA_INDEX_SIZE_IN_UINT32 ];
}


uint64_t getFileSize(
    const char *filename )
{
    FILE *f = 0;
    uint64_t fileSize = 0;

    if ( filename == NULL)
        return 0;

    f = fopen( filename, "rb" );

    if ( f == NULL )
    {
        return 0;
    }
#if WINDOWS
    _fseeki64( f, 0, SEEK_END );
    fileSize = _ftelli64( f );
#else
    fseek( f, 0, SEEK_END );
    fileSize = ftell( f );
#endif

    fclose( f );

    return fileSize;
}



uint32_t param[ AFA_PARAM_BLOCK_SIZE_IN_BYTES / sizeof( uint32_t )];

int main(
    int argc,
    char* argv[])
{
    AFASpectra_SW *spectraDataInput;
    uint32_t *spectraDataInputHW;
    AFASpectra_SW *spectraDataWorkingSet;
    uint32_t *spectraDataWorkingSetHW;
    AFAReadBackData_t *readBackData;
    uint32_t *baseAddr = NULL;  // that's a dummy address pointing to the start of work area of HW in memory
    uint32_t xp, yp, gridSize, gridSizeSqr;
    sint32_t idx = 0;
    int rv = 0;
    bool_t rc;

    uint64_t spectraDataInputHW_OffsetToBaseAddress = 0x0;
    uint64_t spectraDataWorkingSetHW_OffsetToBaseAddress = 0x0;
    uint64_t pSpectraIndexList_OffsetToBaseAddress = 0x0;
    uint64_t readBackData_OffsetToBaseAddress = 0x0;

    uint32_t numSpectra;        // do we have more than 4bn spectra ... ? not in THESE times
    uint32_t remainder = 0;     // must kept zero, otherwise wrong
    uint32_t srcDataSelector = 1;

    char dumpFilename[] = "TESTforFPGAclasses.bin";
    uint64_t dumpFileSize = 0;

    uint32_t breakAfterNumCyles;
    clock_t timeGlobalStart;
    clock_t timeGlobalEnd;
    clock_t timeIterationStart;
    clock_t timeIterationEnd;

//    uint64_t test;

	printf( "Starting main() ...\n" );

    // processor and other HW preparations --- start ------------------------------------------------------

#ifdef AFA_RUN_ON_XILINX_SDK
    Xil_ICacheEnable();
	Xil_DCacheEnable();

    Xil_AssertSetCallback( AssertCallback );

    DriverInterruptControllerInit();
    DriverTimerInit();
    DriverAFAProcessingInit();
    DriverInterruptControllerEnable();
	LEDInit();

	TestClockFunction();
#endif

    // processor and other HW preparations --- end --------------------------------------------------------
#if 0
	for(;;)
	{
		static uint32_t n = 1000;
	    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_GREEN );		// power on
	    sleep( n );
	    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_BLUE );			// power on
	    sleep( n );
	}
#endif
    LEDRGBSet( 0, EVAL_BOARD_LEDRGB_GREEN );		// power on
    if ( !AFATypesOK())
    {
        printf( "Error with AFATypes.h\n" );
		LEDRGBSet( 0, EVAL_BOARD_LEDRGB_RED );		// error
		LEDRGBSet( 0, EVAL_BOARD_LEDRGB_YELLOW );	// ASSERT
		LEDRGBSet( 0, EVAL_BOARD_LEDRGB_YELLOW );	// ASSERT
		LEDRGBSet( 0, EVAL_BOARD_LEDRGB_YELLOW );	// ASSERT

        exit( 1 );
    }

    // clear param
    memset( param, 0x42, AFA_PARAM_BLOCK_SIZE_IN_BYTES );

    switch ( srcDataSelector )
    {
        default:
        case 0: // simple sine spectra
        {
            printf( "* Get number of spectra from sine calculation\n" );

            numSpectra = 100;  // do we have more than 4bn spectra ... ? not in THESE times
            break;
        }
        case 1: // 100 Spectra from Philipp
        {
            printf( "* Get number of spectra from array (Philipp)\n" );
            dumpFileSize = testAFAshort_size;
            numSpectra   = ( uint32_t )( dumpFileSize / sizeof( AFASpectra_SW ));
            remainder    = ( uint32_t )( dumpFileSize % sizeof( AFASpectra_SW ));
            if ( numSpectra == 0 || remainder != 0 )
            {
                printf( "SpectraArray wrong size.\n" );
                exit( 1 );
            }
            break;
        }
        case 2: // load from array
        {
            printf( "* Get number of spectra from array\n" );
//            dumpFileSize = getSpectraArraySize();
            numSpectra   = ( uint32_t )( dumpFileSize / sizeof( AFASpectra_SW ));
            remainder    = ( uint32_t )( dumpFileSize % sizeof( AFASpectra_SW ));
            if ( numSpectra == 0 || remainder != 0 )
            {
                printf( "SpectraArray wrong size.\n" );
                exit( 1 );
            }
            break;
        }
        case 3: // load from file
        {
            printf( "* Get number of spectra from file\n" );
            dumpFileSize = getFileSize( dumpFilename );
            numSpectra   = ( uint32_t )( dumpFileSize / sizeof( AFASpectra_SW ));
            remainder    = ( uint32_t )( dumpFileSize % sizeof( AFASpectra_SW ));
            if ( numSpectra == 0 || remainder != 0 )
            {
                printf( "Dump file not found or wrong file size.\n" );
                exit( 2 );
            }
            break;
        }
    }

    printf( "* Set default parameters\n" );
    AFASetDefaultParameters( &AFAPP_sw.m_params );
#ifdef JSCDBG_ITER_SPECIAL
    AFAPP_sw.m_params.numSteps = 200;
#else
    AFAPP_sw.m_params.numSteps = 1;
#endif
    AFAPP_sw.m_params.searchMode = AFANET_SETTINGS_SEARCHMODE_global;

    printf( "* Prepare data structures\n" );
    AFAHelperStructures_PrepareDataStructure(
        numSpectra );
    printf( "* Allocate memory\n" );
    rc = AFAHelperStructures_MemAllocate();
    if ( !rc )
    {
    	printf( "!!! Error: No memory \n" );
    	exit( -1 );
    }
    printf("* Set baseAddr\n");
    baseAddr = AFAPP_sw.memoryBlockBaseAddressAligned;
    printf("  - baseAddr = 0x%16.16llx\n", baseAddr );

    printf( "* Update address data\n" );
    AFAHelperStructures_UpdateAddressData();
    spectraDataInput = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf(
        "example data" );
    spectraDataInputHW = ( uint32_t * ) AFAHelperStructures_GetAddressOf(
        "example data reduced" );
    spectraDataWorkingSet = ( AFASpectra_SW * ) AFAHelperStructures_GetAddressOf(
        "m_pNet / SOM" );
    spectraDataWorkingSetHW = ( uint32_t * ) AFAHelperStructures_GetAddressOf(
        "m_pNet / SOM reduced" );
    readBackData = ( AFAReadBackData_t * ) AFAHelperStructures_GetAddressOf(
    	"readBackData" );

    gridSize = AFAPP_sw.m_gridSize;
    gridSizeSqr = AFAPP_sw.m_gridSizeSqr;

    switch ( srcDataSelector )
    {
        default:
        case 0: // simple sine spectra
        {
            printf( "* Generate sine test spectra\n" );
            generateSineTestSpectra(
                numSpectra,
                spectraDataInput );

            printf( "* Convert spectra records: input data\n" );
            break;
        }
        case 1:	// spectra uploaded to 0x90000000
        {
            printf( "* Read spectra array (Philipp)\n" );
            // copy test data to correct memory location
            memcpy( spectraDataInput, testAFAshort_data, testAFAshort_size );

            break;
        }
        case 2: // load from array
        {
            printf( "* Read spectra array\n" );
//            if ( readSpectraArrayToHWSpectra( numSpectra, spectraDataInputHW ) == FALSE )
            {
                printf( "Error processing TestDataArray.\n" );
                exit( 3 );
            }
            break;
        }
        case 3: // load from file
        {
            printf( "* Load spectra from file\n" );
            if ( readSpectraFileToHWSpectra( dumpFilename, numSpectra, spectraDataInputHW ) == FALSE )
            {
                printf( "Error reading dump file.\n" );
                exit( 4 );
            }
            break;
        }
    }

    printf( "* Init processing (new)\n" );
    AFAInitProcessingNew(
        FALSE );

    // convert data to new layout
    swSpectraToHwSpectra(
        spectraDataInput,
        spectraDataInputHW,
        numSpectra );
    printf( "* Convert spectra records: working set data\n" );
    swSpectraToHwSpectra(
        spectraDataWorkingSet,
        spectraDataWorkingSetHW,
        gridSizeSqr );

    AFARandomInitRandom(
        AFAPP_sw.m_params.randomSeed );

    // here we convert pointer differences to byte offsets (baseAddr is NULL)
    spectraDataInputHW_OffsetToBaseAddress      = (( uint64_t )( addr_t )( char * ) AFAPP_sw.spectraDataInputHW      ) - (( uint64_t )( addr_t )( char * ) baseAddr );
    spectraDataWorkingSetHW_OffsetToBaseAddress = (( uint64_t )( addr_t )( char * ) AFAPP_sw.spectraDataWorkingSetHW ) - (( uint64_t )( addr_t )( char * ) baseAddr );
    pSpectraIndexList_OffsetToBaseAddress       = (( uint64_t )( addr_t )( char * ) AFAPP_sw.m_pSpectraIndexList     ) - (( uint64_t )( addr_t )( char * ) baseAddr );
    readBackData_OffsetToBaseAddress			= (( uint64_t )( addr_t )( char * ) readBackData                     ) - (( uint64_t )( addr_t )( char * ) baseAddr );

    // initialize readBackData
    memset(( void * ) readBackData, 0x00, sizeof( AFAReadBackData_t ));

    // define here after how many iterations we want to see a result
#ifdef AFA_TEST_SHORT_ITERATION_0000
    breakAfterNumCyles = 2;
#else
    breakAfterNumCyles = 0xffffffff;
#endif

    printf( "* Iterate over working set data\n" );
    timeGlobalStart = clock();
    do
    {

#ifdef AFA_RUN_ON_XILINX_SDK
	Xil_DCacheEnable();
#endif

        if ( currentStep > AFAPP_sw.m_params.numSteps )
        {
            //Clustering finished (success).
            rc = TRUE;
        }
        else
        {
            float32_t lPercent = ( float32_t )( currentStep ) / ( float32_t )( AFAPP_sw.m_params.numSteps );
            float32_t lRate = ( float32_t ) ( AFAPP_sw.m_params.lRateBegin * powf( AFAPP_sw.m_params.lRateEnd / AFAPP_sw.m_params.lRateBegin, lPercent ));
            float32_t adaptionThreshold = AFAPP_sw.m_params.lRateEnd * 0.01f;
            float32_t sigma = ( float32_t ) ( AFAPP_sw.m_params.radiusBegin * powf( AFAPP_sw.m_params.radiusEnd / AFAPP_sw.m_params.radiusBegin, lPercent));
            float32_t sigmaSqr = sigma*sigma;
            uint32_t i;
            bool_t bFullSearch = TRUE;
            uint32_t searchRadius = 1;

            // determine search strategy for BMUs for the current learning step
            if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_localfast )
            {
                // always use a constant search radius, never do a global search
                bFullSearch = ( currentStep < 1 );
                searchRadius = 2;
            }
            else if ( AFAPP_sw.m_params.searchMode == AFANET_SETTINGS_SEARCHMODE_local )
            {
                // global search for the first 5 steps, decreasing search radius for increasing number of learning steps
                bFullSearch = ( currentStep < 5 );
                searchRadius = ( unsigned int )((( 1.f - lPercent ) * 0.5f * ( float )( AFAPP_sw.m_gridSize ))) + 2;
            }
            else // SOFMNET_SETTINGS_SEARCHMODE_global
            {
                // always use global search, never go local.
                // slow but guarantees optimal results in every case
                bFullSearch = TRUE;
            }

            // ==================================================
            //
            // prepare HW data structures (swizzle indices, etc.)
            //
            // ==================================================

            // store all indices in a list
            for ( i = 0; i < numSpectra; i++ )
            {
                AFAPP_sw.m_pSpectraIndexList[ i ] = i;
            }

            // shake well
            for ( i = 0; i < numSpectra * 2; i++ )
            {
                uint32_t ind0 = AFARandomIntRange( numSpectra - 1 );
                uint32_t ind1 = AFARandomIntRange( numSpectra - 1 );

                // switch indices
                sint32_t tmp = AFAPP_sw.m_pSpectraIndexList[ ind0 ];
                AFAPP_sw.m_pSpectraIndexList[ ind0 ] = AFAPP_sw.m_pSpectraIndexList[ ind1 ];
                AFAPP_sw.m_pSpectraIndexList[ ind1 ] = tmp;
            }

            // clear names
            for ( i = 0; i < gridSizeSqr * AFA_SPECTRA_INDEX_SIZE_IN_UINT32; i += AFA_SPECTRA_INDEX_SIZE_IN_UINT32 )
            {
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_LOW  ] = 0;
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_SPEC_OBJ_ID_HIGH ] = 0;
                AFAPP_sw.spectraDataWorkingSetHW[ i + AFA_SPECTRA_INDEX_INDEX            ] = ( uint32_t ) ( -1 );
            }

            // ==================================================
            //
            // prepare parameter block
            //
            // ==================================================

            param[ AFA_PARAM_INDICES_FULL_SEARCH                       ] = bFullSearch;
            param[ AFA_PARAM_INDICES_SEARCH_RADIUS                     ] = searchRadius;
            param[ AFA_PARAM_INDICES_ADAPTION_THRESHOLD                ] = *(( uint32_t * ) &adaptionThreshold );
            param[ AFA_PARAM_INDICES_SIGMA_SQR                         ] = *(( uint32_t * ) &sigmaSqr          );
            param[ AFA_PARAM_INDICES_LRATE                             ] = *(( uint32_t * ) &lRate             );
            param[ AFA_PARAM_INDICES_GRID_SIZE                         ] = gridSize;
            param[ AFA_PARAM_INDICES_GRID_SIZE_SQR                     ] = gridSizeSqr;
            param[ AFA_PARAM_INDICES_NUM_SPECTRA                       ] = numSpectra;
            param[ AFA_PARAM_INDICES_RNG_MTI                           ] = m_mti;
            param[ AFA_PARAM_INDICES_PIXEL_START                       ] = 0;
            param[ AFA_PARAM_INDICES_PIXEL_END                         ] = 0;
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_LOW    ] = ( uint32_t )((             spectraDataInputHW_OffsetToBaseAddress      )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INPUT_HW_ADDR_HIGH   ] = ( uint32_t )((( uint64_t ) spectraDataInputHW_OffsetToBaseAddress      ) >> 32 );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_LOW       ] = ( uint32_t )((             spectraDataWorkingSetHW_OffsetToBaseAddress )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_WS_HW_ADDR_HIGH      ] = ( uint32_t )((( uint64_t ) spectraDataWorkingSetHW_OffsetToBaseAddress ) >> 32 );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_LOW  ] = ( uint32_t )((             pSpectraIndexList_OffsetToBaseAddress       )       );
            param[ AFA_PARAM_INDICES_SPECTRA_DATA_INDEX_LIST_ADDR_HIGH ] = ( uint32_t )((( uint64_t ) pSpectraIndexList_OffsetToBaseAddress       ) >> 32 );
            param[ AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_LOW           ] = ( uint32_t )((             readBackData_OffsetToBaseAddress            )       );
            param[ AFA_PARAM_INDICES_READ_BACK_DATA_ADDR_HIGH          ] = ( uint32_t )((( uint64_t ) readBackData_OffsetToBaseAddress            ) >> 32 );

            // ==================================================
            //
            // call HW calculation
            //
            // ==================================================

            timeIterationStart = clock();
            rc = AFAProcess_HWWrapper(
                param,                              // whole block ram used
                baseAddr
                );
            timeIterationEnd = clock();
            printf( "[%3d/%d = %3d%%] IterTime: %ldms [%7.2fsec.]\n",
                currentStep,
                AFAPP_sw.m_params.numSteps,
                ( currentStep * 100 ) / ( AFAPP_sw.m_params.numSteps + 1 ),
                timeIterationEnd - timeIterationStart,
                (( float64_t )( timeIterationEnd - timeIterationStart )) / (( float64_t ) CLOCKS_PER_SEC ));
            if ( !rc )
            {
                currentStep++;
            }
        }
        // =======================================================
        // important:
        // here we wait - in absense of a memory fence - for
        // the memory write pipeline to be empty
	    //sleep( 1000 );
        // =======================================================

    } while (( !rc ) && ( --breakAfterNumCyles ));
    timeGlobalEnd = clock();
    printf( "GlobalTime: %ldms [%7.2fsec.]\n",
		timeGlobalEnd - timeGlobalStart,
		(( double )( timeGlobalEnd - timeGlobalStart )) / (( double ) CLOCKS_PER_SEC ));

#ifdef AFA_RUN_ON_XILINX_SDK
	Xil_DCacheDisable();
#endif

#if 0
#if 1

    idx = 0;
	test = baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + 64 ];
	do
    {
    	idx++;
    } while ( baseAddr[ AFA_PARAM_BLOCK_ADDRESS_INDEX + 64 ] == test );

#else
    idx = 0;
    do
    {
    	idx++;
    } while ( idx < 5000000 );
#endif
#endif
	printf( "\nStatistics [%d]:\n", idx );
    printf( "* memAccess_AFAProcess_HW:              [%10llu]\n", readBackData->stats.memAccess_AFAProcess_HW );
    printf( "* memAccess_adaptNetwork_HW_read:       [%10llu]\n", readBackData->stats.memAccess_adaptNetwork_HW_read );
    printf( "* memAccess_adaptNetwork_HW_write:      [%10llu]\n", readBackData->stats.memAccess_adaptNetwork_HW_write );
    printf( "* memAccess_searchBestMatchComplete_HW: [%10llu]\n", readBackData->stats.memAccess_searchBestMatchComplete_HW );

    rv = 0;
    switch ( srcDataSelector )
    {
        default:
        case 0: // simple sine spectra
        {
			sint32_t idx_golden;
				
			// print results
			gridSize = AFACalcGridSize(numSpectra);

			printf( "\nFinal result !\n" );
			for ( yp = 0; yp < gridSize; yp++ )
			{
				printf( "\t{" );
				for ( xp = 0; xp < gridSize; xp++ )
				{
					idx = AFAGetSpectraIndexNew( xp, yp );
					idx = ( idx < 0 ) ? -1 : idx;
					idx_golden = golden_data[ yp ][ xp ];
					if ( idx != idx_golden )
					{
						rv = 1000000 + xp + yp * gridSize;

						if ( idx < 0 )
							printf( "*  -1* " );
						else
							printf( "*%4d* ", idx );
					}
					else
					{
						if ( idx < 0 )
							printf( "   -1, " );
						else
							printf( " %4d, ", idx );
					}
				}
				printf( "},\n" );
			}
			printf( "\n" );
            break;
        }
        case 1: // simple sine spectra
        {
            sint32_t idx_golden;
#ifdef AFA_TEST_SHORT_ITERATION_0000
            uint32_t *p = ( uint32_t * )testAFAshortIndex0001_data; // beware of the endianess
#else
            uint32_t *p = ( uint32_t * )testAFAshortIndex0200_data; // beware of the endianess
#endif
            // print results
            gridSize = AFACalcGridSize(numSpectra);

            printf( "\nFinal result !\n" );
            for ( yp = 0; yp < gridSize; yp++ )
            {
                printf( "\t{" );
                for ( xp = 0; xp < gridSize; xp++ )
                {
                    idx = AFAGetSpectraIndexNew( xp, yp );
                    idx = ( idx < 0 ) ? -1 : idx;
					idx_golden = p[ yp * gridSize + xp ];
                    if ( idx != idx_golden )
                    {
                        rv = 1000000 + xp + yp * gridSize;

                        if ( idx < 0 )
                            printf( "*  -1* " );
                        else
                            printf( "*%4d* ", idx );
                    }
                    else
                    {
                        if ( idx < 0 )
                            printf( "   -1, " );
                        else
                            printf( " %4d, ", idx );
                    }
                }
                printf( "},\n" );
            }
            printf( "\n" );
            break;
        }
        case 2: // load from array
        case 3: // load from file
        {
#if 0
			uint32_t *p = ( uint32_t * )afaTestDataGoldenResult_data; // beware of the endianess

			sint32_t idx_golden;
				
			// print results
			gridSize = AFACalcGridSize(numSpectra);

			printf( "\nFinal result !\n" );
			for ( yp = 0; yp < gridSize; yp++ )
			{
				printf( "\t{" );
				for ( xp = 0; xp < gridSize; xp++ )
				{
					idx = AFAGetSpectraIndexNew( xp, yp );
					idx = ( idx < 0 ) ? -1 : idx;
					idx_golden = p[ yp * gridSize + xp ];
					if ( idx != idx_golden )
					{
						rv = 1000000 + xp + yp * gridSize;

						if ( idx < 0 )
							printf( "* -1* " );
						else
							printf( "*%4ld* ", idx );
					}
					else
					{
						if ( idx < 0 )
							printf( "   -1, " );
						else
							printf( " %4ld, ", idx );
					}
				}
				printf( "},\n" );
			}
			printf( "\n" );

		    {
		        uint32_t *p = ( uint32_t * )afaTestDataGoldenResult_data; // beware of the endianess

		        sint32_t idx_golden;

		        printf( "Correct solution:\n" );

		        // print results
		        gridSize = AFACalcGridSize(numSpectra);
		        for ( yp = 0; yp < gridSize; yp++ )
		        {
		            printf( "\t{" );
		            for ( xp = 0; xp < gridSize; xp++ )
		            {
		                idx_golden = p[ yp * gridSize + xp ];
		                if ( idx_golden < 0 )
		                    printf( "   -1, " );
		                else
		                    printf( " %4ld, ", idx_golden );
		            }
		            printf( "},\n" );
		        }
		        printf( "\n" );
		    }
#endif
            break;
        }
    }

    printf( "\n" );

    AFAHelperStructures_MemFree();

    if ( rv > 0 )
    {
        printf( "ERROR!!!: %d\n", rv - 1000000 );
    }
    else
    {
        printf( "==> Error free !!!\n" );
#ifdef JSCDBG_ACCEPT_LITTLE_INACCURACIES
        printf( "    ==> Remember we accept little inaccuracies !!!\n" );
#endif
    }
    return rv;
}
