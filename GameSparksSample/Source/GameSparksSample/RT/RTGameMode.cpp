#include "GameSparksSample.h"
#include "RTGameMode.h"

#include <GameSparksRT/RTData.hpp>
#include <GameSparks/generated/GSMessages.h>

#include <string> // for std::to_string()
#include <cstdlib> // for rand()
#include <ctime> // for std::time
#include <sstream>

/// 1. In your GameMode's constructor initialize the GameSparks Component:
ARTGameMode::ARTGameMode()
:nextMessageInSeconds(1.0f)
{
    //Initialise GameSparksComponent used to connect
    GameSparksComp = CreateDefaultSubobject<UGameSparksComponent>(TEXT("GSparkComp"));
}


/// 2. In your GameMode's BeginPlay function add your handler function to the OnGameSparksAvailableDelegate invocation list then call the Connect function from your GameSparks Component:
void ARTGameMode::BeginPlay() {

    Super::BeginPlay();
    //Add a function to the OnGameSparksAvailableDelegate invocation list
    GameSparksComp->OnGameSparksAvailableDelegate.AddDynamic(this, &ARTGameMode::OnGameSparksAvailable);
    //Connect to GameSparks using Key and Secret
    GameSparksComp->Connect("exampleKey01", "exampleSecret0123456789012345678");
}


void ARTGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

    // disconnect from GameSparks
    gameSession.Reset();
    GameSparksComp->Disconnect();
}


/// here we implement a IRTSessionListener that holds a IRTSession and prints to the screen
void ARTGameMode::OnPlayerConnect(int peerId)
{
    std::stringstream ss;
    ss << " OnPlayerConnect:" << peerId;
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ss.str().c_str());
}

void ARTGameMode::OnPlayerDisconnect(int peerId)
{
    std::stringstream ss;
    ss << " OnPlayerDisconnect:" << peerId;
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ss.str().c_str());
}

void ARTGameMode::OnReady(bool ready)
{
    std::stringstream ss;
    ss << " OnReady:" << std::boolalpha << ready;
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ss.str().c_str());
}

void ARTGameMode::OnPacket(const GameSparks::RT::RTPacket& packet)
{
    gsstl::stringstream ss;
    ss << "OnPacket: " << packet.Data;
    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, ss.str().c_str());
}


void ARTGameMode::Tick(float DeltaSeconds)
{
    AGameMode::Tick(DeltaSeconds);

    if (gameSession.IsValid())
    {
        gameSession->Update();

        // update our timer
        nextMessageInSeconds -= DeltaSeconds;
        if(nextMessageInSeconds <= 0.0f) // is it expired ?
        {
            nextMessageInSeconds = 1.0f; // reset the timer

            // send a message to all peers.
            GameSparks::RT::RTData data;
            data.SetString(1, "Hello Real-Time Services"); // do something useful here
            gameSession->SendRTData(1, GameSparks::RT::GameSparksRT::DeliveryIntent::RELIABLE, data, {});
        }
    }
}

/// 3. Finally just add logic to your handler function to execute code when GameSparks becomes available:
/// Available => Authenticate => Matchmaking => MatchFound


using namespace GameSparks::Core;
using namespace GameSparks::Api::Messages;
static void OnMatchFound(GS& gs, const MatchFoundMessage& message)
{

}

//OnAvailable Handler
void ARTGameMode::OnGameSparksAvailable(bool available) {

    if (available) {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Connected"));
        //Get a pointer to the module's GSInstance
        GameSparks::Core::GS& gs = UGameSparksModule::GetModulePtr()->GetGSInstance();

        // attach the message listeners
        // MatchFoundMessage
        gs.SetMessageListener<MatchFoundMessage>([&](GS& gsInstance, const MatchFoundMessage& message) {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("MatchFoundMessage"));

            gameSession = TSharedPtr<GameSparks::RT::IRTSession>(GameSparks::RT::GameSparksRT::SessionBuilder()
                .SetConnectToken(message.GetAccessToken().GetValue())
                .SetHost(message.GetHost().GetValue())
                .SetPort(message.GetPort().GetValue())
                .SetListener(this)
                .Build());

            gameSession->Start();
        });

        // MatchNotFoundMessage
        gs.SetMessageListener<MatchNotFoundMessage>([&](GS& gsInstance, const MatchNotFoundMessage& message) {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("MatchNotFoundMessage"));
            SendMatchmakingRequest(gs); // try again
        });

        // MatchUpdatedMessage
        gs.SetMessageListener<MatchUpdatedMessage>([&](GS& gsInstance, const MatchUpdatedMessage& message) {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, TEXT("MatchUpdatedMessage"));
        });

        //Construct an Authentication request
        // send a device authentication request with a random device id.
        // this is done so that you can start two instances on the same machine.
        // In a production title, you'd use IGSPlatform::GetDeviceId() instead.
        GameSparks::Api::Requests::DeviceAuthenticationRequest authRequest(gs);
        std::srand(std::time(nullptr));
        gsstl::stringstream fake_device_id;
        fake_device_id << std::rand();
        authRequest.SetDeviceId(fake_device_id.str()); // generate a random device id (for easy testing)
        authRequest.SetDeviceOS("W8");
        authRequest.SetUserData(this);

        //Send it with a pointer to the function that will handle the response
        authRequest.Send(AuthenticationRequest_Response);
    }
}

//Example response function
void ARTGameMode::AuthenticationRequest_Response(GameSparks::Core::GS&, const GameSparks::Api::Responses::AuthenticationResponse& response)
{
    GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, response.GetJSONString().c_str());
    //Check is response has no errors
    if (!response.GetHasErrors()) {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("Auth response successful"));

        GameSparks::Core::GS& gs = UGameSparksModule::GetModulePtr()->GetGSInstance();

        SendMatchmakingRequest(gs);
    }
}


void ARTGameMode::SendMatchmakingRequest(GameSparks::Core::GS& gs)
{
    GameSparks::Api::Requests::MatchmakingRequest makeMatch(gs);
    makeMatch.SetSkill(1);
    makeMatch.SetMatchShortCode("TestMatch");
    makeMatch.Send(MatchmakingRequest_Response);
}


void ARTGameMode::MatchmakingRequest_Response(GameSparks::Core::GS&, const GameSparks::Api::Responses::MatchmakingResponse& response)
{
    if (response.GetHasErrors())
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, response.GetErrors().GetValue().GetJSON().c_str());
    }
}
