#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace esbroot+;
#pragma link C++ namespace esbroot::generators+;
#pragma link C++ namespace esbroot::generators::generic+;
#pragma link C++ namespace esbroot::generators::superfgd+;
#pragma link C++ namespace esbroot::generators::ndcherenkov+;

#pragma link C++ class esbroot::generators::superfgd::GenieGenerator+;
#pragma link C++ class esbroot::generators::superfgd::GenieFluxDriver+;
#pragma link C++ class esbroot::generators::superfgd::FgdGeomAnalyzer+;
#pragma link C++ class esbroot::generators::superfgd::FgdGenieGenerator+;

#pragma link C++ class esbroot::generators::generic::GenieGenerator+;
#pragma link C++ class esbroot::generators::generic::GenieFluxDriver+;
#pragma link C++ class esbroot::generators::generic::GenericGeomAnalyzer+;
#pragma link C++ class esbroot::generators::generic::GenericGenieGenerator+;
#pragma link C++ class esbroot::generators::generic::IFluxNextPosition+;
#pragma link C++ class esbroot::generators::generic::CompositeIFluxNextPosition+;

#pragma link C++ class esbroot::generators::ndcherenkov::NDSimpleGenerator+;

#endif
