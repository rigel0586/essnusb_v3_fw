{
    printf("\n Loading libraries ... \n\n");

    // 1. load data
    std::stringstream ss;
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/build/data/libessnusb_data.so";
    std::string data_pcm = ss.str();

    printf("\n %s \n", data_pcm.c_str());
    gSystem->Load(data_pcm.c_str());

    ss.str(std::string());
    ss.clear();

    // 2. Load utility
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/build/utility/libessnusb_utility.so";
    std::string utility_pcm = ss.str();
    printf("\n %s \n", utility_pcm.c_str());
    gSystem->Load(utility_pcm.c_str());


    ss.str(std::string());
    ss.clear();

    // 3. Load geometry
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/build/geometry/libessnusb_geometry.so";
    std::string geometry_pcm = ss.str();
    printf("\n %s \n", geometry_pcm.c_str());
    gSystem->Load(geometry_pcm.c_str());

    ss.str(std::string());
    ss.clear();

    // 4. Load generators
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/build/generators/libessnusb_generators.so";
    std::string generators_pcm = ss.str();
    printf("\n %s \n", generators_pcm.c_str());
    gSystem->Load(generators_pcm.c_str());


    ss.str(std::string());
    ss.clear();

    // 5. Load core
    ss << gSystem->Getenv("ESB_BASE_DIR");
    ss << "/build/core/libEsbCore.so";
    std::string core_pcm = ss.str();
    printf("\n %s \n", core_pcm.c_str());
    gSystem->Load(core_pcm.c_str());
}