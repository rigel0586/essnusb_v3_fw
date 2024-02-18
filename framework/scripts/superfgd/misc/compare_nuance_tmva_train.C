void compare_nuance_tmva_train(TString nuanceFile = "/home/rigel/opt/ESSnuSB/ESSnuSB-soft/EsbMacro/tests/nuFluxHist/data_04_01_2021/SFGD_WC_crossover_events_220107/crossover_neg_numu.dat"            // Path to statsRoot.root generated from simulate_3_MC_lepton_stats.C
                        , TString tmvaFile = "/home/rigel/opt/ESSnuSB/ESSnuSB-soft/EsbMacro/tests/nuFluxHist/data_04_01_2021/negative/nuFlux100km_250kAm_mu_tmva_train.dat"      // output file where to write the nuance formatted data
                        )                                     
{
    /// Reading tmva data file
    ///==========================================================================
    std::map<Int_t, Double_t> bdtgValues;
    std::map<Int_t, Double_t> TMlpANNValues;


    const char tabChar('\t');
    std::ifstream tmvaFileFileStream(tmvaFile, std::ios::in);
    
    try
    {
        if(tmvaFileFileStream.is_open())
        {
            std::string line;
            const int skipInitialLines = 1;
            int countLines = 0;
            while(std::getline(tmvaFileFileStream,line))
            {
                if(countLines < skipInitialLines)
                {
                    std::cout << "Skipping " << line << std::endl;
                    ++countLines;
                    continue;
                }

                int arrInd(0);

                std::istringstream ss(line);
                std::string token;

                Int_t eventNum = 0; 
                Double_t parsedValBtg = 0;
                Double_t parsedValTMlp = 0;
                bool hasValue{false};
                //std::cout << "Line " << line << std::endl;
                while((arrInd<3) && std::getline(ss, token, tabChar))
                {
                    //std::cout << "token " << token << std::endl;
                    if(!token.empty())
                    {
                        if(arrInd == 0) 
                        {
                            //bdtgValues.emplace_back(parsedVal);
                            eventNum = std::stoi(token);
                        } 
                        else if(arrInd == 1) 
                        {
                            //bdtgValues.emplace_back(parsedVal);
                            parsedValBtg = std::stod(token);
                        } 
                        else if(arrInd == 2) 
                        {
                            //TMlpANNValues.emplace_back(parsedVal);
                            parsedValTMlp = std::stod(token);
                            hasValue = true;
                        }

                        ++arrInd;
                    }
                }


                if(hasValue)
                {
                    bdtgValues[eventNum] = parsedValBtg;
                    TMlpANNValues[eventNum] = parsedValTMlp;
                }

                ++countLines;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception while reading!" << std::endl;
    } 

    //for(size_t i = 0; i < bdtgValues.size(); ++i)
    {
        //std::cout << bdtgValues[i] << std::endl;
    }

    //for(size_t i = 0; i < TMlpANNValues.size(); ++i)
    {
        //std::cout << TMlpANNValues[i] << std::endl;
    }

    if(tmvaFileFileStream.is_open())
    {
        tmvaFileFileStream.close();
    }
    ///==========================================================================


    /// Reading nuance data file
    ///==========================================================================
    //std::vector<Double_t> nuanceValues;
    std::map<Int_t, Double_t> map_nuanceValues;

    const char spaceChar(' ');
    std::ifstream nuanceFileFileStream(nuanceFile, std::ios::in);
    
    try
    {
        if(nuanceFileFileStream.is_open())
        {
            std::string line;
            const int skipInitialLines = 1;
            int countLines = 0;
            while(std::getline(nuanceFileFileStream,line))
            {
                if(countLines < skipInitialLines)
                {
                    std::cout << "Skipping " << line << std::endl;
                    ++countLines;
                    continue;
                }

                int arrInd(0);

                std::istringstream ss(line);
                std::string token;
                
                Int_t eventNUm = 0;
                Double_t parsedVal = 0;
                bool hasValue{false};
                
                //std::cout << "Line " << line << std::endl;
                while((arrInd<2) && std::getline(ss, token, spaceChar))
                {
                    //std::cout << "token " << token << std::endl;
                    if(!token.empty())
                    {
                        if(arrInd == 0) 
                        {
                            eventNUm = std::stoi(token);
                            //nuanceValues.emplace_back(parsedVal);
                        } 
                        if(arrInd == 1) 
                        {
                            parsedVal = std::stod(token);
                            hasValue = true;
                            //nuanceValues.emplace_back(parsedVal);
                        } 
                        ++arrInd;
                    }
                }

                if(hasValue)
                {
                    map_nuanceValues[eventNUm] = parsedVal;
                }

                ++countLines;
            }
        }
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception while reading!" << std::endl;
    } 

    //for(size_t i = 0; i < nuanceValues.size(); ++i)
    //{
        //std::cout << nuanceValues[i] << std::endl;
    //}


    if(nuanceFileFileStream.is_open())
    {
        nuanceFileFileStream.close();
    }
    ///==========================================================================


    /// Generating histogram
    ///==========================================================================
    TCanvas* fcanvas = new TCanvas();
    TH2F* f_hist = new TH2F("hist","Nuance/TMVA histogram",100,0.75,1.25,100,-1.5,1.5);

    // for(size_t i = 0; (i < nuanceValues.size()) && (i < TMlpANNValues.size()); ++i)
    // {
    //     f_hist->Fill(nuanceValues[i], TMlpANNValues[i], 1.0);
    // }

    auto iter = map_nuanceValues.begin();
    for(; iter!= map_nuanceValues.end(); ++iter)
    {
        auto contains = TMlpANNValues.find(iter->first);
        if(contains != TMlpANNValues.end() && iter->second != 1.0)
        {
            f_hist->Fill(iter->second, TMlpANNValues[iter->first], 1.0);
        }
    }

    f_hist->Draw();

    fcanvas->Update();
    fcanvas->Draw();
    fcanvas->Write(); ;
    fcanvas->SaveAs("hist_Nuance_TMlpANN neg_mu histogram.png");


    f_hist->Reset();
    iter = map_nuanceValues.begin();
    for(; iter!= map_nuanceValues.end(); ++iter)
    {
        auto contains = bdtgValues.find(iter->first);
        if(contains != bdtgValues.end() && iter->second != 1.0)
        {
            f_hist->Fill(iter->second, bdtgValues[iter->first], 1.0);
        }
    }

    f_hist->Draw();

    fcanvas->Update();
    fcanvas->Draw();
    fcanvas->Write(); ;
    fcanvas->SaveAs("hist_Nuance_BTG neg_mu histogram.png");

    fcanvas->ResetDrawn();
    ///==========================================================================
}
