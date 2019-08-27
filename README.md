# Dandelion2D

Hi! This is a 2D game engine and sample game made using the engine.
The purpose of this project was as a challenge for myself to create a game engine and game in the shortest amount of time possible.
Originally, the goal was to create a *game* in under 12 hours in a custom engine, but I got carried away with writing different parts of the engine such that it turned into a game engine with a game created using the engine.
  
I didn't keep very close track of time, but here's a rough breakdown of time usage in this project:

#### Game Engine Hours
* Core (math library, memory management, input handling, defines and type definitions) : 3 hours
* Graphics (openGL rendering) : 4 hours
* Physics (impulse based physics engine) : 7 hours
* Audio (portaudio custom audio callback) : 5 hours
* GameObject Management/Factory : 7 hours
  
*Total* : ~26 +/- 4 hours
  
#### Game Hours
* Game Application/State management : 3 hours
* Game Scripts : 5 hours
* Game Levels/Design : 4 hours
  
*Total* : ~12 +/- 2 hours
  
#### Total Time worked on both game and game engine: approximately 38 +/- 6 hours
  
## Game Demo
The game is a simple token collection game. The player must move around the level collecting the floating/rotating purple squares while avoiding hazards. 
The player is equipped with the ability to fire a bullet that will change gravity to be centered around the object that it strikes. 
Left clicking will fire a gravity-altering bullet, and right-clicking will restore the gravity to normal. After three levels, the player wins! The player has five lives per level.

## Build Instructions
* *Option 1*: Simply open the .sln file to open both Visual Studio 2017 projects, and you can build and run from there.
* *Option 2*: A built version of the game resides at `Dandelion2D\bin\Release-x64\Game\Game.exe`. 
* *Option 3*: To use the engine without the demo (not sure why you would), you can either copy the folder `Dandelion2D\bin\Release-x64\Dandelion2D` which contains the .lib and .dll needed for building and linking, or copy `Dandelion2D\Dandelion2D` which contains both the source code and Visual Studio .vcxproj files needed to add this to an existing .sln.
  
## Engine Design Notes
Notes here are in the order that they were written in. 
  
### Defines and types
* Basic defines for dll import/export, assert and static_assert, alignment, and memory usage.
* Also included typedefs of the basic types for clarity.
  
### Math
* I knew I wanted to write a custom vec2 library to practice using simd operations.
* The rest of the math library was delegated to GLM to avoid wasting time.
* There was a bunch of helper math-related macros that I pulled from another project relating to lerps, clamping, random numbers, etc.
  
### Memory
* In game engines, cache is king, and I knew that I would have major slowdowns in the graphics department, so carefully managing my memory was going to be important.
* I wrote a simple pool allocator to do this and keep all my memory contiguous.
  
### GameObject
* Defined the beginnings of game objects early on. 
* GameObjects are defined as a collection of behaviors, position/rotation/scale, and a sprite.
* Sprite is just what color of square the object is, because of how I set up graphics.
* Physics bodies are managed and updated separately from the GameObject for quicker iteration.

### GameObject management
* GameObjects were all stored in a pool allocator, and in a name map.
* The name map didn't quite work out as I intended, as GameObjects stored a const char* for their name instead of a std::string, so making clones of archetypes messed the whole system up. I didn't have time to fix that issue, so it remained until the end.
* GameObject deletion is pretty unstable, so the final game is prone to crashes. 
  
### Graphics
* I've never written a graphics engine before (my background is in audio programming), so I knew that trying to write one as fast as possible was going to be difficult and bad.
* I pulled up learnopengl.com, and went through the process of rendering a triangle and rectangle to the screen.
* At this point, it was 2am of the first night, I had put 2 hours of work into getting the .libs to link, nothing was rendering properly, so I went to bed.
* The next morning, I could think clearly, and got all the GameObjects rendering on the screen using a draw call per GameObject. 
* I didn't waste time with trying to render textures, I wanted to keep things simple.
* Because the tutorials were all 3D, the camera is actually in 3D space, and rendering things using a perspective matrix, even though the objects are all 2D.
  
### Physics
* The first thing I did was try to pull code from a previous project that I hadn't written to try to implement SAT collision detection. That was a bad idea. I wasted about 2 hours trying to pull old code from other projects and get it working, before I stopped and wrote it all from scratch.
* I used Randy Gaul's "[How to Create a Custom 2D Physics Engine](https://gamedevelopment.tutsplus.com/series/how-to-create-a-custom-physics-engine--gamedev-12715)" tutorial to implement the narrow phase and resolution code.
* Collision resolution isn't perfect, i.e. objects tend to sink into each other, but it works well enough.
* Only uses AABBs and Circles. 
* Played around with gravity to let the user set custom gravities, and it inspired the game demo main mechanic.
  
### Audio
* It took an unexpected long time to build the right x64 version of portaudio that would actually work how I wanted it to.
* I never got panning working, but that's okay.
* Pitch shift is grainy, and pops, but works with shifting by cents. Never used though, so not really important.

### Input
* Used GLFW for input polling. Nothing to note here.
  
### Scripting
* I decided that in order to make behaviors, I would just let users add scripts like in Unity to GameObjects.
* Scripts would be defined on the client side to be attached to GameObjects.
* Scripts are just a name, a map of floats with names (per script variables), a map of void pointers with names (per script references), and three lambdas for Update, OnCollisionEnter, and OnCollisionExit.
* Scripts are pretty easy to use and made making the game pretty easy!
  
### GameObject factory
* Because of how objects are allocated, I couldn't make an inheritance tree of subclasses of GameObjects. So I had to store constructed GameObjects as achetypes by name.
* When instantiating a GameObject from an archetype, I needed to give the new object a unique name. However, because GameObjects store their names as const char*, I couldn't just append a "-Copy" to the name, which is what I did anyway, and it definitely backfired. It's definitely a bug that I haven't bothered to fix because of time. 
  
## Final Takeaways
* Writing a graphics engine from scratch as fast as possible without having ever written one was not a great idea.
* Optimizing the audio engine to successfully play more than one voice was both satisfying and rewarding.
* std::set is a surprisingly slow container! It was responsible for slowing down the physics engine to be slower than graphics. Removing std::set inserts and inlining all vec2 functions brought collision checking to be faster than calling glDrawElements on every sprite in the game every frame.
* Making a 2D engine in this manner was very enjoyable! I was able to use pool allocators for many things and got pretty good speed ups because of that.
* Doing this same challenge for a 3D engine would probably take in the realm of 50 to 100 hours. This was definitely a difficult and consuming project. 
