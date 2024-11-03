void simulate_2_process(TString wcsim_simulation_file_path="/home/rigel/essnusb/working_repo/fw/essnusb_v3_fw/framework/workspace/simulation/wcsim_sim_output.root")
{
    TFile* tf = new TFile(wcsim_simulation_file_path);
    TTree *tree = (TTree*)tf->Get("WCSim_Detector");

    if(tree == nullptr){
        cout << " Tree  'WCSim_Detector' is not present. Exiting ...         "<< endl;
        return;
    }

    TClonesArray* wcsimData = nullptr;
    tree->SetBranchAddress("DataSimulationPoints", &wcsimData);

    int entries = tree->GetEntries();
    for(int i=0; i < entries; ++i) 
    { 
        cout << "                 Event " << i << "           "<< endl;
        tree->GetEntry(i); 
        const Int_t points = wcsimData->GetEntries();
        for(Int_t j =0; j < points; j++)
        {
            esbroot::data::wcsim::WCSimDataPoint* wcsimPoint = (esbroot::data::wcsim::WCSimDataPoint*)wcsimData->At(i);
            cout << "           Point number " << j << "           "<< endl;
            cout << "                   Pdg  " << wcsimPoint->GetPdg() << endl;
            cout << "         FreePathLength " << wcsimPoint->GetTrackLenght() << endl;
            cout << "                EventId " << wcsimPoint->GetEventID() << endl;
            cout << "                TrackID " << wcsimPoint->GetTrackID() << endl;
            cout << "               ParentID " << wcsimPoint->GetParentID() << endl;
            cout << "           ParticleName " << wcsimPoint->GetParticleName() << endl;

            TVector3 pos = wcsimPoint->GetPosition();
            cout << "           Position X " << pos.X() << endl;
            cout << "           Position Y " << pos.Y() << endl;
            cout << "           Position Z " << pos.Z() << endl;

            TVector3 mom = wcsimPoint->GetMomentum();
            cout << "           Momentum X " << mom.X() << endl;
            cout << "           Momentum Y " << mom.Y() << endl;
            cout << "           Momentum Z " << mom.Z() << endl;

            cout << "              StepTime " << wcsimPoint->GetStepTime() << endl;
            cout << "           ProcessName " << wcsimPoint->GetProcessName() << endl;
            cout << "               SecName " << wcsimPoint->GetSecName() << endl;
            
            cout << "                   Px " << wcsimPoint->GetPx() << endl;
            cout << "                   Py " << wcsimPoint->GetPy() << endl;
            cout << "                   Pz " << wcsimPoint->GetPz() << endl;

            cout << "                    X " << wcsimPoint->GetX() << endl;
            cout << "                    Y " << wcsimPoint->GetY() << endl;
            cout << "                    Z " << wcsimPoint->GetZ() << endl;

            cout << "=======================================" << endl;
        }
        
    }
}


