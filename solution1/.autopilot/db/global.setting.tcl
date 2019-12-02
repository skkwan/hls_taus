
set TopModule "myproject"
set ClockPeriod "4.120000"
set ClockList {ap_clk}
set multiClockList {}
set PortClockMap {}
set CombLogicFlag 0
set PipelineFlag 1
set DataflowTaskPipelineFlag  1
set TrivialPipelineFlag 0
set noPortSwitchingFlag 0
set FloatingPointFlag 0
set FftOrFirFlag 0
set NbRWValue 0
set intNbAccess 0
set NewDSPMapping 1
set HasDSPModule 1
set ResetLevelFlag 1
set ResetStyle "control"
set ResetSyncFlag 1
set ResetRegisterFlag 0
set ResetVariableFlag 0
set fsmEncStyle "onehot"
set maxFanout "0"
set RtlPrefix ""
set ExtraCCFlags ""
set ExtraCLdFlags ""
set SynCheckOptions ""
set PresynOptions ""
set PreprocOptions ""
set SchedOptions ""
set BindOptions ""
set RtlGenOptions ""
set RtlWriterOptions ""
set CbcGenFlag ""
set CasGenFlag ""
set CasMonitorFlag ""
set AutoSimOptions {}
set ExportMCPathFlag "0"
set SCTraceFileName "mytrace"
set SCTraceFileFormat "vcd"
set SCTraceOption "all"
set TargetInfo "xc7vx690t:ffg1927:-2"
set SourceFiles {sc {} c {../../hls_taus/parameters.h ../../hls_taus/myproject.h ../../hls_taus/myproject.cpp ../../hls_taus/BDT.h ../../hls_taus/input.txt ../../hls_taus/PF_Tau.hpp ../../hls_taus/PF_Tau.cc ../myproject.cpp ../PF_Tau.cc}}
set SourceFlags {sc {} c {{ } { } { -std=c++0x} { -std=c++0x} { } { } { } -std=c++0x {}}}
set DirectiveFile {/afs/cern.ch/user/s/skkwan/Public/triggerDevel/hls_taus/solution1/solution1.directive}
set TBFiles {verilog ../myproject_test1.cpp bc ../myproject_test1.cpp sc ../myproject_test1.cpp vhdl ../myproject_test1.cpp c {} cas ../myproject_test1.cpp}
set SpecLanguage "C"
set TVInFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TVOutFiles {bc {} c {} sc {} cas {} vhdl {} verilog {}}
set TBTops {verilog {} bc {} sc {} vhdl {} c {} cas {}}
set TBInstNames {verilog {} bc {} sc {} vhdl {} c {} cas {}}
set XDCFiles {}
set ExtraGlobalOptions {"area_timing" 1 "clock_gate" 1 "impl_flow" map "power_gate" 0}
set PlatformFiles {{DefaultPlatform {xilinx/virtex7/virtex7 xilinx/virtex7/virtex7_fpv6}}}
set DefaultPlatform "DefaultPlatform"
set TBTVFileNotFound "../myproject_test1.cc"
set AppFile "../vivado_hls.app"
set ApsFile "solution1.aps"
set AvePath "../.."
set HPFPO "0"
