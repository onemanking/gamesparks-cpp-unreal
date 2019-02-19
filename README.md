## Gamesparks Cpp Unreal

The official GameSparks plugin for Unreal

## Prerequisites (depending on which platforms you want to support)
1. [Unreal Engine 4.16+](https://www.unrealengine.com/what-is-unreal-engine-4)
2. [Android NDK](https://developer.android.com/tools/sdk/ndk/index.html#download)
3. [XCode](https://developer.apple.com/xcode/downloads/)
4. [Visual Studio](http://www.visualstudio.com/downloads/download-visual-studio-vs.aspx)


## Running the sample

1. double click GameSparksSample/GameSparksSample.uproject
2. open the GameSparks GameMode blueprint and set the API credentials
3. press play in the Unreal Editor


## Integrating the SDK into your project

1. Copy the Plugin located at plugin/GameSparks into the Plugins directory of your project
2. Add the "GameSparks" module to PublicDependencyModuleNames in your project.Build.cs
3. Copy GameSparksGameMode.uasset into the Content directory of your project
4. Adapt the blueprint to your needs

## Real-Time Services

There are currently two ways to use the Real-Time Services: Via Blueprints Via C++ code. The sample-project contains two levels in the Rt subdirectory that illustrate both scenarios.
If you want to use the Real-Time Services via C++, also have a look sample07Realtime.cpp of the base SDK. [The tutorials for the Real-Time Services](https://docs.gamesparks.com/tutorials/real-time-services/)
are a great place to start as well. The API of the C++ SDK is very simmilar to the .NET SDK, so most of the sample code should be easy to adapt.

## Online Subsystem

The Unreal Plugin also contains a module that implements parts on Unreals Online Subsystem interface. To Use the OnlineSubsystemGameSparks add the following lines to your DefaultEngine.ini:

[OnlineSubsystemGameSparks]
bEnabled=True
ApiKey=<your-api-key>
ApiSecret=<your-api-secret>
bUsePreviewServer=True

Make sure to encrypt your .pak files to make it more difficult to steal the ApiSecret.
Be aware that those settings override the Key and Secret used in BluePrints.

## License

This library is licensed under the Apache 2.0 License. 
