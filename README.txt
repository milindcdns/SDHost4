
This release includes documentation, core-driver source code for the provided Cadence IP.

A Quick Start Guide is provided in the doc/ directory, we suggest you read this first.

Documentation is available in the doc/ directory, as follows:
  doc/core_driver -  User guides for all the drivers in the package
  doc/test_reports - Test results for the core driver
  doc/porting -      Porting guide for the core driver

Core driver source is available in the core_driver/ directory.  Please see the documentation for details of how to use this.
  core_driver/include - public C headers, for inclusion in your code
  core_driver/src - private C headers and the source code
  core_driver/common - all common include files (such as: cdn_log.h, cps.h, errno.h, stdtypes.h,...)

Combo PHY source is available in the ccp_driver/ directory.  Please see the documentation for details of how to use this.
  ccp_driver/include - public C headers, for inclusion in your code
  ccp_driver/src - private C headers and the source code
  ccp_driver/common - all common include files 

Reference source is available in the reference_code/ directory.
  reference_code/tests - sample code showing how to initialize the SD Host driver and how to program/read memory card in various controller modes

