#pragma once

#include "GameFramework/GameMode.h"

/// 1. Include the following headers:
#include "GameSparksModule.h"
#include <GameSparks/GS.h>
#include <GameSparks/generated/GSResponses.h>
#include <GameSparks/generated/GSRequests.h>

#include <GameSparksRT/IRTSession.hpp>
#include <GameSparksRT/IRTSessionListener.hpp>
using GameSparks::RT::IRTSessionListener;

#include "RTGameMode.generated.h"

/**
 * This Sample shows how to establish a connection to GameSparks via C++ in Unreal. It also shows basic
 * usage of the Real-Time Services. To run it, export it from the editor and start it, you can then
 * press play in the editor and the two instances should connect. This example sends a string message onece
 * a second to the remote peers.
 *
 *  The basic steps for using the GameSparks SDK from C++ are also
 *  outlined here: https://docs.gamesparks.com/tutorials/game-engine-integrations/unreal-cpp-quick-start.html
 */
UCLASS()
class GAMESPARKSSAMPLE_API ARTGameMode : public AGameMode, public IRTSessionListener {
        GENERATED_BODY()

        /// 3. Add your GameMode's constructor and begin play. With it a UFUNCTION with a bool parameter to handle the OnGameSparksAvailableDelegate invocation:

        //Constructor
        ARTGameMode();

        // BeginPlay
        virtual void BeginPlay() override;

        // EndPlay
        virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

        // Tick
        virtual void Tick(float DeltaSeconds) override;

        //Function used to determine what happens if GameSparks connects or fails to (Needs to be UFUNCTION)
        UFUNCTION()
        void OnGameSparksAvailable(bool available);
	
    private:
        /// 2. Create a variable for the GameSparks component:
        TWeakObjectPtr<UGameSparksComponent> GameSparksComp;

        // resonse callbacks & helpers
        static void AuthenticationRequest_Response(GameSparks::Core::GS&, const GameSparks::Api::Responses::AuthenticationResponse& response);
        static void SendMatchmakingRequest(GameSparks::Core::GS& gs);
        static void MatchmakingRequest_Response(GameSparks::Core::GS&, const GameSparks::Api::Responses::MatchmakingResponse& response);

        /// implementation of the IRTSessionListener Interface
        void OnPlayerConnect(int peerId) override;
        void OnPlayerDisconnect(int peerId) override;
        void OnReady(bool ready) override;
        void OnPacket(const GameSparks::RT::RTPacket& packet) override;

        // the IRTSession is created when a match was found.
        TSharedPtr<GameSparks::RT::IRTSession> gameSession;
        float nextMessageInSeconds;
};
