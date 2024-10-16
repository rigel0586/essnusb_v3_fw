void simulate_2_process(TString nd_simulation_file_path)
{
    TFile* tf = new TFile(nd_simulation_file_path);
    TTree *tree = (TTree*)tf->Get("Gd_Cherenkov_Detector");

    if(tree == nullptr){
        cout << " Tree  'Gd_Cherenkov_Detector' is not present. Exiting ...         "<< endl;
        return;
    }

    TClonesArray* ndData = nullptr;
    tree->SetBranchAddress("DataSimulationPoints", &ndData);

    int entries = tree->GetEntries();
    for(int i=0; i < entries; ++i) 
    { 
        cout << "                 Event " << i << "           "<< endl;
        tree->GetEntry(i); 
        const Int_t points = ndData->GetEntries();
        for(Int_t j =0; j < points; j++)
        {
            esbroot::data::ndcherenkov::NDCherenkovDataPoint* ndPoint = (esbroot::data::ndcherenkov::NDCherenkovDataPoint*)ndData->At(i);
            cout << "           Point number " << j << "           "<< endl;
            cout << "                   Pdg  " << ndPoint->GetPdg() << endl;
            cout << "         FreePathLength " << ndPoint->GetTrackLenght() << endl;
            cout << "                EventId " << ndPoint->GetEventID() << endl;
            cout << "                TrackID " << ndPoint->GetTrackID() << endl;
            cout << "               ParentID " << ndPoint->GetParentID() << endl;
            cout << "           ParticleName " << ndPoint->GetParticleName() << endl;

            TVector3 pos = ndPoint->GetPosition();
            cout << "           Position X " << pos.X() << endl;
            cout << "           Position Y " << pos.Y() << endl;
            cout << "           Position Z " << pos.Z() << endl;

            TVector3 mom = ndPoint->GetMomentum();
            cout << "           Momentum X " << mom.X() << endl;
            cout << "           Momentum Y " << mom.Y() << endl;
            cout << "           Momentum Z " << mom.Z() << endl;

            cout << "              StepTime " << ndPoint->GetStepTime() << endl;
            cout << "           ProcessName " << ndPoint->GetProcessName() << endl;
            cout << "               SecName " << ndPoint->GetSecName() << endl;
            
            cout << "                   Px " << ndPoint->GetPx() << endl;
            cout << "                   Py " << ndPoint->GetPy() << endl;
            cout << "                   Pz " << ndPoint->GetPz() << endl;

            cout << "                    X " << ndPoint->GetX() << endl;
            cout << "                    Y " << ndPoint->GetY() << endl;
            cout << "                    Z " << ndPoint->GetZ() << endl;

            cout << "=======================================" << endl;
        }
        
    }
}


