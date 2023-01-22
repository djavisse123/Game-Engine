# Game-Engine
This is a project I made in one of my Computer Science classes at NC State. The course was titled Game Engine Foundations.
In this course we spent our time over the semester building a Game Engine with the goal of being able to use this engine to make a game at the end of the semester.
The game engine I was able to create comes with a couple of features like multiplayer, event management, and scripting. In this project there are two folders.
The first is called 2D Platformer. This folder features the base of my engine where I used it to create a basic platformer. The second folder is called
Breakout which features a version of the game Breakout that I used my game engine to make. Some files and classes were modified in order to create this game but
most of systems used in my original game engine are still available.

## NOTE
All the characters, graphics, and music used in my game engine **DO NOT BELONG TO ME**. They are from the game series Kirby which is developed by HAL Laboratory and
is published by Nintendo. All assets belong to them and are only used to demonstrate my game working with different visuals.

# 2D Platformer

## COMPILATION & RUNNING 
In order to run this project, you will need to download the files and run it on a Linux system like **Ubuntu**.
There are also a couple of libraries that are necessary to run the project. Check the **libraries** section to make sure you have everything.

To compile this project just type in ```make``` in the command line. The makefile compiles two executables. 
One titled **main** and other titled **gameEngineServer**. 
To run this project you must first use ```./gameEngineServer``` to start the server. 
Then in another terminal you can use ```./main``` to run the game engine.

The makefile for this project does not belong to me and was provided by the course.

## LIBRARIES
For this project I used four libraries. The first being the Simple and Fast Multimedia Library(SFML) which is used to be able to create the graphics used in this engine.
I installed this libaray using the command ```sudo apt install libsfml-dev```

The second being the ZMQ Networking library. I installed this library using the command ```sudo apt-get install libzmq3-dev```
In the makefile, the flag -lzmq is included to compile this project using this library.

The third library included in this project is nlohmann's JSON for Modern C++. 
For this library I used the single-include file and inside my main.cpp and gameEngineServer I use ```#include "nlohmann/json.hpp"```
This should compile without any issues.

The fourth library is V8 scripting library. I installed this library using ```sudo apt update && sudo apt -y install build-essential libnode-dev```
In the makefile, the flag -lv8 is included to compile this project using this library.

## CONTROLLING THE GAME
To move the character, you can use the **arrow keys**.

If you want to pause the game, then press the **P** key to pause.

If you want to change the speed of the character the you can you the **period** and **comma** keys. The period key can speed up the game up to 2 times speed and the comma key can slow down the game to 0.5 times speed.

To make the character jump you can use the **spacebar**.

## MULTIPLAYER
To run this game in a multiplayer environment, all you have to do is run another instance of the game engine in a different terminal using ```./main```
## Scripts
Scripts are used in this project and function like cheats. To first active cheats press the key combination ```Left Ctrl + Left Arrow + Right Arrow + Spacebar```
Once this combination is pressed, a message on your screen should display saying **"cheats active"**
To disable cheats just press the key combination ```Left Ctrl + Left Arrow + Right Arrow.``` Once cheats are disabled the **"cheats active"** message should go away.

* change_managment_speed.js: This script increases the speed of the character. To run this script just press the **F** key when cheats are active.
* increase_lives.js: This script increases the number of lives the character has to 99. To run this script just press the **L** key when cheats are active.
* spawn_event_script.js: This script creates an event with the type SPAWN_EVENT and causes the character to return to spawn. To run this script just press the S when cheats are active.
* resest_lives.js: This script resets the number of lives of the character back to its default. To run this script just press **K** key when cheats are active. This script also automatically runs when cheats are disabled.
* reset_movement_speed.js: This script rests the character's speed to its default values. To run this script just press the **G** key when cheats are active. This script also automatically runs when cheats are disabled.
* Reset All: If you want both the amount of lives and the character's speed then just press the **R** key when cheats are active. This will call both the reset_lives and reset_movement_speed scripts.

## Demo
If you are unable to run this project but still want to see what it looks like when ran, then you can watch this video showing a little demo of the project.
This demo features some cheats being used and a demonstration of multiplayer being used with 4 clients.

https://www.youtube.com/watch?v=BDjk78K7BTw


# Breakout

## COMPILATION & RUNNING 
In order to run this project, you will need to download the files and run it on a Linux system like **Ubuntu**.
There are also a couple of libraries that are necessary to run the project. Check the **libraries** section to make sure you have everything.

To compile this project just type in ```make``` in the command line. The makefile compiles two executables. 
One titled **main** and other titled **gameEngineServer**. 
To run this project you must first use ```./gameEngineServer``` to start the server. 
Then in another terminal you can use ```./main``` to run the game engine.

The makefile for this project does not belong to me and was provided by the course.

## LIBRARIES
For this project I used four libraries. The first being the Simple and Fast Multimedia Library(SFML) which is used to be able to create the graphics used in this engine.
I installed this libaray using the command ```sudo apt install libsfml-dev```

The second being the ZMQ Networking library. I installed this library using the command ```sudo apt-get install libzmq3-dev```
In the makefile, the flag -lzmq is included to compile this project using this library.

The third library included in this project is nlohmann's JSON for Modern C++. 
For this library I used the single-include file and inside my main.cpp and gameEngineServer I use ```#include "nlohmann/json.hpp"```
This should compile without any issues.

The fourth library is V8 scripting library. I installed this library using ```sudo apt update && sudo apt -y install build-essential libnode-dev```
In the makefile, the flag -lv8 is included to compile this project using this library.

## CONTROLLING THE GAME
This game is a version of Atari Breakout. When you first start the ball will be on the paddle and you can move the paddle left and right to decide where you want to launch the ball from the paddle.

To move the paddle, use the left and right **arrow keys** to move left and right.

When the paddle's position is towards the left half of the screen the ball will be launched in the upright direction. When the paddle's position is towards the right half of the screen the ball will move to the upleft direction when launched.

To launch the ball, press the **spacebar** and the ball will begin to move.

To pause the game, press the **P** key to pause.

If you want to change the speed of the game, then you can you the **period** and **comma** keys. The **period** key can speed up the game up to 2 times speed and the **comma** key can slow down the game to 0.5 times speed.

The game features 4 levels with an endless mode at the end when you beat all 4 levels. The goal is to try and get the highest score.

This game is entirely single player and does not feature any type of multiplayer. 

## Scripts 
Scripts are used in this project and function like cheats. To first active cheats press the key combination ```Left Ctrl + Left Arrow + Right Arrow + Spacebar```
Once this combination is pressed, a message on your screen should display saying "cheats active"
To disable cheats just press the key combination ```Left Ctrl + Left Arrow + Right Arrow.``` Once cheats are disabled the "ceats active message should go away.

* change_managment_speed.js: This script increases the speed of the ball. To run this script just press the **F** key when cheats are active.
* slow_character_movement.js: This script decreases the speed of the ball. To run this script just press the **D** key when chears are active.
* increase_lives.js: This script increses the number of lives the ball has to 99. To run this script just press the **L** key when cheats are active.
* spawn_event_script.js: This script creates an event with the type SPAWN_EVENT and causes the ball to return to spawn. To run this script just press the **S** when cheats are active.
* resest_lives.js: This script resets the number of lives of the ball back to it's default. To run this script just press **K** key when cheats are active. This script also automatically runs when cheats are disabled.
* reset_movement_speed.js: This script rests the ball's speed to its default values. To run this script just press the **G** key when cheats are active. This script also automatically runs when chears are disabled.
* Reset All: If you want both the amount of lives and the ball's speed then just press the **R** key when cheats are active. This will call both the reset_lives and reset_movement_speed scripts.

## Demo of the Game
If you are unable to run this project but still want to see what it looks like when ran, then you can watch this video showing a little demo of the project.
This demo features the use of some of the cheats being used as well as a demonstration of how to play the game and the levels that are available to play.

https://youtu.be/s8xS84bU5ag
