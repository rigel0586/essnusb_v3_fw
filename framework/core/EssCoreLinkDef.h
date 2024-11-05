#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace esbroot+;
#pragma link C++ namespace esbroot::core+;
#pragma link C++ namespace esbroot::core::io+;
#pragma link C++ namespace esbroot::core::physicsList+;
#pragma link C++ namespace esbroot::core::detector+;
#pragma link C++ namespace esbroot::core::generator+;
#pragma link C++ namespace esbroot::core::simulation+;
#pragma link C++ namespace esbroot::core::task+;
#pragma link C++ namespace esbroot::core::eve+;

#pragma link C++ class esbroot::core::io::EsbIO+;
#pragma link C++ class esbroot::core::io::EsbWriterPersistency+;
#pragma link C++ class esbroot::core::io::EsbReaderPersistency+;

#pragma link C++ class esbroot::core::physicsList::ESSnusbPhysicsList+;

#pragma link C++ class esbroot::core::detector::EsbDetectorConstructor+;
#pragma link C++ class esbroot::core::detector::IDetector+;

#pragma link C++ class esbroot::core::generator::IGenerator+;

#pragma link C++ class esbroot::core::simulation::EsbSimManager+;
#pragma link C++ class esbroot::core::simulation::EsbRunAction+;
#pragma link C++ class esbroot::core::simulation::EsbEventAction+;
#pragma link C++ class esbroot::core::simulation::EsbActionInitializer+;

#pragma link C++ class esbroot::core::task::EsbTaskManager+;
#pragma link C++ class esbroot::core::task::ITask+;

#pragma link C++ class esbroot::core::eve::IEvent+;
#pragma link C++ class esbroot::core::eve::ITrack+;
#pragma link C++ class esbroot::core::eve::ITrackPoint+;
#pragma link C++ class esbroot::core::eve::EsbEveManager+;
#pragma link C++ class esbroot::core::eve::EsbEveEventManager+;
#pragma link C++ class esbroot::core::eve::EsbViewerManager+;

#endif


