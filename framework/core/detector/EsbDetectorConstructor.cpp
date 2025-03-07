#include "EsbDetectorConstructor.hpp"
ClassImp(esbroot::core::detector::EsbDetectorConstructor)

#include <fairlogger/Logger.h>

#include "G4Types.hh"
#include "G4Version.hh"
#include "G4ScaledSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4PVPlacement.hh"

namespace esbroot {
namespace core {
namespace detector {

EsbDetectorConstructor::EsbDetectorConstructor(const std::string& workDir, std::vector<IDetector*>& detectors, GeomConverter converter)
    : fWorkDir(workDir), fDetectors(detectors), fConverter(converter)
{
}

EsbDetectorConstructor::EsbDetectorConstructor(const std::string& gdml_view_File) : f_dgml_view_File(gdml_view_File)
{

}

EsbDetectorConstructor::~EsbDetectorConstructor()
{
}


G4VPhysicalVolume* EsbDetectorConstructor::Construct()
{
    G4VPhysicalVolume* worldvolume = nullptr;
    if(f_dgml_view_File.empty())
    {
        worldvolume = Create();
    }
    else
    {
        worldvolume = fIo.readGdmlToGeant4(f_dgml_view_File);
    }

    if(f_isView)
    {
        AddTransparency(worldvolume,0.75);
        if (fOverlapCheck) {
            std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid*>> overlaps = CheckOverlap(worldvolume);
            DrawOverlap(overlaps);
        }
        // Turn world volume visible
        worldvolume->GetLogicalVolume()->SetVisAttributes(G4VisAttributes(true, G4Colour(1,1,1,0.1)));
    }

    return worldvolume;
}



G4VPhysicalVolume* EsbDetectorConstructor::Create()
{
    using namespace std::placeholders;

    for(IDetector* d : fDetectors)
    {
        d->ConstructGeometry(); // Every detector constructs its volumes and add it to the Cern root Top volume
    }

    std::string fileGdml = fWorkDir + "/" + fgdml;
    fIo.ExportTGeoVolume(fileGdml);
    std::string fileRoot = fWorkDir + "/" + froot;
    fIo.ExportTGeoVolume(fileRoot);

    G4VPhysicalVolume* convertedWorld = fIo.readGdmlToGeant4(fileGdml);

    for(IDetector* d : fDetectors)
    {
        d->PostConstructG4Geometry(convertedWorld); // Pass G4 world if any post convertion configuration is required
    }

    if(fConverter == GeomConverter::G4Root){
        std::string filePostGdml = fWorkDir + "/" + fPostgdml;
        fIo.ExportG4Volume(filePostGdml, convertedWorld);

        if(!fIo.ImportTGeoVolume(filePostGdml)){
            LOG(error) << "Unable to convert geometry to root format";
            exit(0);
        }

        fIo.deleteFile(filePostGdml);
    } 
    else if(fConverter == GeomConverter::VGM){
        std::string fileVGMWorld = fWorkDir + "/" + fPostVgmRoot;
        fIo.ExportG4VolumeVGM(fileVGMWorld, convertedWorld); // Already imports into gGeoManager
        // if(!fIo.ImportTGeoVolume(fileVGMWorld)){
        //     LOG(error) << "Unable to convert geometry to root format";
        //     exit(0);
        // }
    }
    else{
        LOG(error) << "Converter Not supported! Exiting ...";
        exit(0);
    }

    std::string fileFinalGdml = fWorkDir + "/" + fFinalgdml;
    fIo.ExportTGeoVolume(fileFinalGdml);
    std::string fileFinalRoot = fWorkDir + "/" + fFinalroot;
    fIo.ExportTGeoVolume(fileFinalRoot);

    std::function<void(G4LogicalVolume*, G4VSensitiveDetector*)> f_sd = 
                    std::bind(&EsbDetectorConstructor::SetSensitiveHandler, this, _1, _2);

    std::function<void(std::string, G4VSensitiveDetector*, bool)> f_sd_multi = 
                    std::bind(&EsbDetectorConstructor::SetMultiSensitiveHandler, this, _1, _2, _3);

    convertedWorld = fIo.readGdmlToGeant4(fileFinalGdml);
    for(IDetector* sd : fDetectors)
    {
        sd->AddSensitiveDetector(convertedWorld, f_sd); // Every detector should find its sensitive volume
    }

    for(IDetector* sd : fDetectors)
    {
        sd->AddMultiSensitiveDetector(convertedWorld, f_sd_multi); // Every detector should find its sensitive volume
    }

    return convertedWorld;
}

void EsbDetectorConstructor::SetSensitiveHandler(G4LogicalVolume* logVol, G4VSensitiveDetector* aSD)
{
    if(G4SDManager::GetSDMpointer()->FindSensitiveDetector(aSD->GetName()) == nullptr){
        G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    }

    if(logVol->GetSensitiveDetector() == nullptr){
        G4VUserDetectorConstruction::SetSensitiveDetector(logVol, aSD);
    }
}

void EsbDetectorConstructor::SetMultiSensitiveHandler(std::string logVolName, G4VSensitiveDetector* aSD, bool multi)
{
    if(G4SDManager::GetSDMpointer()->FindSensitiveDetector(aSD->GetName()) == nullptr){
        G4SDManager::GetSDMpointer()->AddNewDetector(aSD);
    }
    G4VUserDetectorConstruction::SetSensitiveDetector(logVolName, aSD, multi);
}
    

void EsbDetectorConstructor::AddDetector(IDetector* d)
{
    if(d != nullptr)
        fDetectors.emplace_back(d);
}

std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid*>>  EsbDetectorConstructor::CheckOverlap(G4VPhysicalVolume* volume
                      , G4int res
                      , G4double tol
                      , G4bool verbose
                      , G4int errMax) 
{
        if (volume->CheckOverlaps(res, tol, verbose, errMax))
            volume->GetLogicalVolume()->SetVisAttributes(G4VisAttributes(G4Colour(1,0,0,0.5)));

        G4int trials = 0;
        G4VSolid* solid = volume->GetLogicalVolume()->GetSolid();
        G4LogicalVolume* motherLog = volume->GetMotherLogical();

        std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid* > > l_Overlaps;
        if (motherLog) 
        {
            G4VSolid* motherSolid = motherLog->GetSolid();
            G4AffineTransform Tm(volume->GetRotation(), volume->GetTranslation());
            for (G4int ires = 0; ires < res; ires++) 
            {
                G4ThreeVector point = solid->GetPointOnSurface();
                G4ThreeVector mp = Tm.TransformPoint(point);

                if (motherSolid->Inside(mp) == kOutside) 
                {
                        G4double distin = motherSolid->DistanceToIn(mp);
                    if (distin > tol) {
                        G4AffineTransform tf1(volume->GetRotation(),volume->GetTranslation());
                        G4AffineTransform tf1i = tf1.Inverse();
                        G4RotationMatrix* rot = new G4RotationMatrix(tf1i.NetRotation());
                        G4ThreeVector trans = tf1i.NetTranslation();
                        G4SubtractionSolid* overlap_solid = new G4SubtractionSolid("overlap_solid", solid, motherSolid, rot, trans);
                        G4ScaledSolid* overlap_scaled_solid = new G4ScaledSolid("overlap_scaled_solid", overlap_solid, G4Scale3D(1.001, 1.001, 1.001));
                        l_Overlaps.push_back(std::make_tuple(volume,overlap_scaled_solid));
                        if (verbose)
                            G4cout << "Overlap of " << volume->GetName() << " with mother " << motherLog->GetName()
                                << " at " << point << " (" << distin/CLHEP::mm << " mm)" << G4endl;

                        if (++trials > errMax) break;
                    }
                }
                auto n = motherLog->GetNoDaughters();
                for (decltype(n) i = 0; i < n; i++) 
                {
                    G4VPhysicalVolume* daughter = motherLog->GetDaughter(i);
                    if (daughter == volume) continue;
                    G4AffineTransform Td( daughter->GetRotation(), daughter->GetTranslation() );
                    G4ThreeVector md = Td.Inverse().TransformPoint(mp);
                    G4VSolid* daughterSolid = daughter->GetLogicalVolume()->GetSolid();
                    if (daughterSolid->Inside(md) == kInside) 
                    {
                        G4double distout = daughterSolid->DistanceToOut(md);
                        if (distout > tol) 
                        {
                            G4AffineTransform tf1(volume->GetRotation(),volume->GetTranslation());
                            G4AffineTransform tf2(daughter->GetRotation(),daughter->GetTranslation());
                            G4AffineTransform tf21 = tf2 * tf1.Inverse();
                            G4RotationMatrix* rot = new G4RotationMatrix(tf21.NetRotation());
                            G4ThreeVector trans = tf21.NetTranslation();
                            G4IntersectionSolid* overlap_solid = new G4IntersectionSolid("overlap_solid", solid, daughterSolid, rot, trans);
                            G4ScaledSolid* overlap_scaled_solid = new G4ScaledSolid("overlap_scaled_solid", overlap_solid, G4Scale3D(1.001, 1.001, 1.001));
                            l_Overlaps.push_back(std::make_tuple(volume,overlap_scaled_solid));
                            if (verbose)
                            G4cout << "Overlap of " << volume->GetName() << " with sister " << daughter->GetName()
                                    << " at " << md << " (" << distout/CLHEP::mm << " mm)" << G4endl;
                            if (++trials > errMax) break;
                        }
                    }
                }    
            }
        }

        return l_Overlaps;
}

void EsbDetectorConstructor::DrawOverlap(std::vector< std::tuple< G4VPhysicalVolume*, G4VSolid*>> overlaps)
{
        for (std::vector< std::tuple<G4VPhysicalVolume*, G4VSolid* > >::const_iterator
                it  = overlaps.begin(); it != overlaps.end(); it++) 
        {
            G4VPhysicalVolume* vol = std::get<0>(*it);
            G4VSolid* sol = std::get<1>(*it);
            G4LogicalVolume* log = new G4LogicalVolume(sol,0,"overlap_log",0,0,0);
            log->SetVisAttributes(G4VisAttributes(G4Colour::Yellow()));
            new G4PVPlacement(0,G4ThreeVector(),"overlap_phys",log,vol,false,0,false);
        }
}

double EsbDetectorConstructor::AddTransparency(G4VPhysicalVolume* volume, G4double alpha)
{
      G4double a = 1;
      auto n = volume->GetLogicalVolume()->GetNoDaughters();
      for (decltype(n) i = 0; i < n; i++) 
        a = std::min(a, AddTransparency(volume->GetLogicalVolume()->GetDaughter(i), alpha));
    
      volume->GetLogicalVolume()->SetVisAttributes(G4VisAttributes(G4Colour(1,1,1,a)));

      return a * alpha;
}

} // namespace detector
} // namespace core
} // namespace esbroot

