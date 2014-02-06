Welcome to ASPECT - A SPEctra Clustering Tool
for the exploration of large spectroscopic surveys

Analysing the output from surveys is an important challenge in modern science. This, however, is not an easy task for a voluminous dataset.

We developed the software tool ASPECT which is based on a novel approach for analysing large spectra databases. The heart of the software is a Kohonen self-organizing map (SOM) that is able to cluster up to several hundred thousand spectra. The resulting two-dimensional SOM of topologically ordered spectra allows the user to browse and navigate through a huge data pool and helps him to gain a deeper insight into underlying relationships between spectral features and other physical properties.

ASPECT supports the visual analysis of the resulting SOM of spectra by the option to blend in individual object properties (e.g., redshift, signal-to-noise ratio, spectral type, morphological type). In addition, the tool provides algorithms for the selection of spectral types, e.g., local difference maps which reflect the deviations of all spectra from a given input spectrum. 


For a detailed description you might want to look into this paper:
ASPECT: A spectra clustering tool for exploration  of large spectral surveys
A.  in der Au, H.  Meusinger, P. F.  Schalldach, M.  Newholm
A&A 547 A115 (2012)
DOI: 10.1051/0004-6361/201219958

A special application is descibed here:
Unusual quasars from the Sloan Digital Sky Survey selected by means of Kohonen self-organising maps⋆
H.  Meusinger, P.  Schalldach, R.-D.  Scholz, A.  in der Au, M.  Newholm, A.  de Hoon, B.  Kaminsky
A&A 541 A77 (2012)
DOI: 10.1051/0004-6361/201118143


This source tree is organized in the following form:

/data                  -  includes some example spectrum files we can import
/export                -  where the exported map is written too
/src/3rdparty          -  3rd party libraries
/src/analyze           -  this is where all the magic happens: calculate SOM and export the clustered map from precompiled list of binary spectra
/src/compare           -  compare one spectrum with clustered map to find similar objects and other sophisticatd foo
/src/dump              -  precompiled spectra (i.e. spectra FITS files from varios SDSS data releases) into a single binary file
/src/sdsslib           -  collection of shared library functions that are used by ASPECT
/src/specObjOperations -  a collection of various special purpose test code 
/src/specObjMapper     -  to plot many spectra into one graph to visually compare them
/src/search            -  perform search queries to find similar spectra for a list of given spectra