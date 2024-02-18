void readTVMAClassTrain()
{
    TFile* tf = new TFile("/home/rigel/opt/ESSnuSB/ESSnuSB-soft/EsbMacro/tests/TMVA_class_Essnusb.root");
    TDirectoryFile* tDir = (TDirectoryFile*)tf->Get("dataset");

    if(tDir != nullptr)
        cout << "         dataset exists " << endl;
    else
    {
        cout << "         dataset does not exists " << endl;
        exit(1);
    }

    TTree* tr = (TTree*)tDir->Get("TestTree");
    if(tr != nullptr)
        cout << "         TestTree exists " << endl;
    else
    {
        cout << "         TestTree does not exists " << endl;
        exit(1);
    }

    int entries = tr->GetEntries();
    cout << "         TestTree entries " << entries << endl;

    TString outSignalFile = "/home/rigel/opt/ESSnuSB/ESSnuSB-soft/EsbMacro/tests/nuFluxHist/data_04_01_2021/negative/nuFlux100km_250kAm_e_tmva_train.dat";      
    TString outBackGroundFile = "/home/rigel/opt/ESSnuSB/ESSnuSB-soft/EsbMacro/tests/nuFluxHist/data_04_01_2021/negative/nuFlux100km_250kAm_mu_tmva_train.dat";
    
    std::ofstream outputSignalFile(outSignalFile, std::ios::trunc);
    std::ofstream outputBackgroundFile(outBackGroundFile, std::ios::trunc);

    if(outputSignalFile.is_open() && outputBackgroundFile.is_open())
	{      // output file where to write the nuance formatted data
        Float_t eventNum(0);
        Float_t bdtg(0);
        Float_t TMlpANN(0);
        Float_t hasExiting(0);
        Int_t classID(2);
        
        tr->SetBranchAddress("eventNum", &eventNum);
        tr->SetBranchAddress("BDTG", &bdtg);
        tr->SetBranchAddress("TMlpANN", &TMlpANN);
        tr->SetBranchAddress("hasExiting", &hasExiting);
        tr->SetBranchAddress("classID", &classID);

        for(int i=0; i < entries; ++i) 
        { 
            tr->GetEntry(i); 
            if(hasExiting > 0)
            {
                cout << "               Entry " << i << "           "<< endl;
                cout << "               eventNum " << eventNum << endl;
                cout << "               BDTG " << bdtg << endl;
                cout << "               TMlpANN " << TMlpANN << endl;
                cout << "               hasExiting " << hasExiting << endl;
                cout << "               classID " << classID << endl;
                cout << "=======================================" << endl;

                if(classID == 0) // 0 is for signal 
                {
                    outputSignalFile << eventNum << "\t" << bdtg << "\t\t" << TMlpANN  << endl;
                }

                if(classID == 1) // 1 is for background
                {
                    outputBackgroundFile << eventNum << "\t" << bdtg << "\t\t" << TMlpANN  << endl;
                }
            }
        }
    }

    outputSignalFile.close();
    outputBackgroundFile.close();
}



