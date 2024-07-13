#pragma once

#ifndef EsbEveEventManager_hpp
#define EsbEveEventManager_hpp 1

#include <vector>
#include <string>
#include <map>

#include "TEveEventManager.h"

#include "IEvent.hpp"
#include "ITrack.hpp"
#include "EsbReaderPersistency.hpp"

namespace esbroot {
namespace core {
namespace eve {

class EsbEveEventManager : public TObject
                    , public TEveEventManager
{
public:

    EsbEveEventManager() = default;
    ~EsbEveEventManager() = default;

    virtual void GotoEvent(Int_t event) override;   // *MENU*
    virtual void NextEvent() override;              // *MENU*
    virtual void PrevEvent() override;              // *MENU*

    void AddEventTracks(int eventId, std::vector<ITrack>& tracks);

private:
    bool translateTracks(int eventId);
    EColor getColor(int pdg);

    int m_currentEventId{0};
    static std::map<int, EColor> m_trackColor;

    std::map<int, std::vector<ITrack>> m_tracks;

    ClassDef(EsbEveEventManager, 2);
};

} // namespace eve
} // namespace core
} // namespace esbroot
#endif