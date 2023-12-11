# UnrealDebugFramework
### NOTE: This is still work in progress and is not yet ready to be released. Bugs, unfinished code and other issues are to be expected at this stage.
A more detail explanation will be added when the product is ready.

A debug framework developed for Unreal which should help streamline how to generate debug content for the game.
It uses external libraries (such as DearImGui) to allow for a more confortable debug experience.
It also allows to generate from code debug classes in an easy way, keeping at a minimun their footprint (no UCLASS or USTRUCTS) and supports multiplayer games as well.

## OVERVIEW:
To be able to display the debug enviroment in game the default keybindings are:
- NumPad + -> Switch between debug camera and control of the player
- NumPad . -> Display the Debug Window
- NumPad 0 -> Switch Input mode from Game to ImGui and viceversa
  
The user can currently create FEATURES and FILTERS.

A FILTER is a statically allocated CPP class which allows the user to select a FEATURE only when the object selected in debug mode passes its conditions.
For example, if the user defines a Filter that returns true only if the object selected is an AI (has an AIController), then if a FEATURE makes use of this, it will only show up in the Debug Selectable Tree when the object selected is an AI (has an AIController).
To create a new Filter the user needs to extend the IKLDebugImGuiFilterInterface (found in KLDebugImGuiUser module) and use the appropriate setup detailed in the .h file. 
Here an example:

```
.h

class FClassFilter final : public IKLDebugImGuiFilterInterface
{
   ...
};

.cpp

KL_DEBUG_CREATE_FILTER(FClassFilter)

```
A FEATURE is a statically allocated CPP class which represent a mechanic we want to display in the ImGui Debug Window.
Features have a path (which needs to respect this format Root.Branch.Branch.Leaf) and are represented in the ImGui Debug Window as a selectable tree.
Here is where the user can define what to display on the ImGui Window that will be created when the Feature is selected.
Features can also make use of FILTERS (by overriding GetFilterPath).
There are 2 types of Interfaces classes that can be extended by the user currently:
  - IKLDebugImGuiFeatureInterface_Selectable: this can be used for objects that can be selected in game (i.e s objects that extend the Actor class).
  - IKLDebugImGuiFeatureInterface_Unique: this can be used for classes which represent managers and have a unique instance during the lifetime of the game. For example this can be used to debug Subsystems.
  - 
To create a new Feature the user needs to extend, for example, the IKLDebugImGuiFeatureInterface_Selectable (found in KLDebugImGuiUser module) and use the appropriate setup detailed in the .h file. Here an example:

```
.h

   class FClassDebug final : public IKLDebugImGuiFeatureInterface_Selectable
   {
      DERIVED_KL_DEBUG_FEATURE_CLASS(FClassDebug, IKLDebugImGuiFeatureInterface_Selectable)
     ...
   };

.cpp

KL_DEBUG_CREATE_WINDOW(FClassDebug)
```

## Missing features: 
- Finish to implement missing parts of code (such as the Picker)
- Commands that the user can send to the server to control its behavior (for example pause the game)
- Feature able to send server update requests from the client only if certain conditions are met (for example, if we are displaying the navmesh around the player, request new navmesh polygons only after the camera has moved a certain amount)
- Finish to implement the Debug camera system. Users should be able to implement their own camera and switch between the different camera modes.
- Add to the Debug Camera HUD a UI that allows to filter and display debug text from BP 
- Detachable ImGui windows from the game window
- Rewrite of the UnrealNetImgui
- Support for remote input (this can be helpfull, for example, when running the game on console)
- Eplore if we can parallelize the code more

## List of Libraries currently been used:

DearImguI: https://github.com/ocornut/imgui

ImGuiNodeEditor: https://github.com/thedmd/imgui-node-editor

ImPlot: https://github.com/epezent/implot

NetImgui: https://github.com/sammyfreg/netImgui

UnrealNetImGui: https://github.com/sammyfreg/UnrealNetImgui

PropertyWatcher: https://github.com/guitarfreak/PropertyWatcher
