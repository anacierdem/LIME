# Large Isometric Map Editor (LIME)

This project once was started to build a map editor for a MMO isometric RPG game but due to the immense amount of work required and my full-time job, it didn't last long :) It was also a nice exercise for me to catch up with some of the newer C++ features.

This is an attempt to reclaim some of the useful ideas and features in this engine. Maybe some of you find it interesting and build upon it someday. For example there is a simple `QuadTree` structure and an arbitrary message passing mechanism among interesting things. Currently this project is kept as a reference only, not maintained actively. I have tried to include comments wherever I feel necessary, but did not do any major refactoring although there is a need for that.

## Basic Project Architecture

The project is based on cross platform multimedia library SFML and browser renderer Awesomium. Hard work was aimed to be done on C++ using SFML whereas Awesomium was being used as a UI library to build the UI easily with our beloved HTML and JS. Unfortunately Awesomium seems to be discontinued for now and this requires that I provide the frameworks and dlls required to link and run the project. I have lost the libraries for Linux so you are on your own if you want to work on it.

One downside of the Awesomium framework at the time was that it is 32 bit only so I needed to re-compile SFML 2.4.0 (which does not have 32 bit OSX binaries for this specific version - as they dropped 32 bit support on OSX at some point). These files are included with the project to be copied to /Library/Frameworks on OSX. On Windows, SFML 2.4.1 is supported. The Visual Studio 2015 solution in the windows folder expects that `AWE_DIR` env. variable pointing to the Awesomium root and `SFML_ROOT` env. variable to point to the SFML root.

In principle, there are two main component types namely `GameObjectComponent` and `EngineComponent`. These are designed such that they can be attached to `GameObject`s and `EngineManager` Respectively. Each `EngineComponent` can communicate through a custom callback mechanism `broadcastMessage` with each other sharing arbitrarily typed messages. They get filtered based on type and invoke their handlers. When passing messages, it is important to note that only childs should send messages upwards whereas parent object should invoke respective APIs in a mediator pattern.

## Building

Once you have the frameworks installed on OSX and libs loaded on Windows you should be able to build and run using the included XCode project or Visual Studio 2015 solution. Also on OSX and Linux, it is possible to compile using the included CMake file.

## Save Mechanism

Every data type that requires a save operation has save/load functions that handle the write operation. in SaveMethods.cpp and `Serialization` namespace there are definitions of save functions for generic types. When there is a save defininition on the class that being saved, that function is invoked. When it does not exist, we will fallback to the generic methods.

## Current Controls

- Mouse wheel: Zoom
- Mouse button: Paint selected tile.
- C: Toggle between player / spectator mode.
- W, A, S, D: Move the player or spectator camera. (isKeyPressed not working on OSX somehow - not sure why, I may be missing the correct 32bit frameworks)
- L: Load map.
- K: Save map.
- N: Generate new random map.
- Q: Send a test message to webview.
- ESC: Quit.
- F5: Reload webview. (Allows for dynamic HTML testing)

## Misc. Information

The webview is composited on top of the GL layer and a shader is used to convert into RGB. HTML and CSS files are in Resources folder. The trees in the view have hard-coded triangular collision regions. The bottom right part of the screen is an in progress collision area editor written with JS. Currently hard-coded paint mode is a large square, you can change this by using `setTile` function instead of `setTileBrush`. The name alEngine comes from the names of the main developers Alper and Ali if you are curious. You can find the required OSX frameworks in the `OSXFrameworks` directory and Windows libraries in `WinLib` folder.

Support me at http://patreon.com/anacierdem
