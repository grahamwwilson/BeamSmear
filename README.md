# README

Make input beam files for Guinea-PIG runs with 
(E, x, y, z, x', y') for each macroparticle in the electron and positron beams 
according to specified Gaussian resolutions and accelerator parameters 
(normalized emittance and beta function)

The tails of the Gaussian distributions are subject to a 
truncation parameter in standard deviations, truncate.
Currently this is the same for all 6 beam variables.

It is suspected that this is needed given that other input files are 
truncated at around 3 standard deviations.

Two configurations thus far.
ILC-250, and ILC-Z.

Independent random number sequences are used for each variable, so 
the seed should be specified in strides of 6 so as to avoid using the same 
seed twice. Example: (1, 7) for e- and e+ for ILC-250, and (13,19) for ILC-Z.

May want to 
  i) increase the number of macroparticles for each beam (N).
 ii) adjust the truncation parameter. Suggest minimum value of 3.0.
