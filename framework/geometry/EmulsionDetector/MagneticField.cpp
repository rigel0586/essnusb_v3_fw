#include "geometry/EmulsionDetector/MagneticField.hpp"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
: G4MagneticField(), fMessenger(0), fBy(0)
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{ 
    delete fMessenger; 
}

void MagneticField::GetFieldValue(const G4double [4], double *bField) const
{
    bField[0] = 0.;
    bField[1] = fBy;
    bField[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::DefineCommands()
{
    // Define /user/field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this, 
                                        "/user/field/", 
                                        "Field control");

    // fieldValue command 
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("value","tesla",
                                  &MagneticField::SetField, 
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("0.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
