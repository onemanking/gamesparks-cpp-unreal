#!/bin/bash

pushd `dirname $0` > /dev/null
SCRIPTPATH=`pwd`
popd > /dev/null

# UE <= 4.11 the script is called RocketGenerateProjectFiles.sh, in more recent versions it's called just GenerateProjectFiles
#cd /Users/Shared/UnrealEngine/4.10/Engine/Build/BatchFiles/Mac/
#./RocketGenerateProjectFiles.sh -project="$SCRIPTPATH/GameSparksSample.uproject" -game

# 4.12 +
cd /Users/Shared/UnrealEngine/UE_4.19/Engine/Build/BatchFiles/Mac/
./GenerateProjectFiles.sh -project="$SCRIPTPATH/GameSparksSample.uproject" -game
