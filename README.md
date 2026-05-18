# The Escapists Inspired Tech Demo (PrisonBreak)
Made in Unreal Engine 5.4.4

PrisonBreak is my take on the Escape brief. It is a prison-break strategy game taking place iniside of a prison where the main character is a prisoner and must escape the prison while minding the guard and other prsisoners. 

It draws inspiration from mainly the escapists for gameplay and other strategy games for the mechanics.

It's key features are the encountered AI the player can face as they have varied designs, the gameplay, combat system, inventory and crafting and the UI. 

---
Some technical Interesting Elements In my Game Are:
- A GOAP Planner (made in C++) that is exposed to the Unreal Editor
- GOAP World State, Actions and Goals (all made in C++) that are also designed to be designer friendly and also designed in editor
- Finite State Machines (C++)
- Behavior Trees (C++)
- Damage and Attack Systems (C++)
- Smart Objects(C++)
- A Custom Blackboard (C++) // depriciated
- Inventory(C++)
- Crafting(C++)
- UI (Somewhere done in Blueprints, some in C++)
- Reactive Animations (the assets were taken from asset stores, edited in the Editor but the functionality came from C++)
- Blackboard (using ue5 in built Blackboard)
- In Game Time System (C++)
- Object Pooling and Management(C++)
- A Signal System, derived from a technical document from the Escapists 2 (C++)
- Utility AI (C++)
- Routine System (C++)
- Data Table Usage
- UE5 built in perception system (logic using this was in C++, configured in blueprints)
---
The Current State Of Play/What has been Implemented So Far :
- The GOAP Planner works
- Actions, Goals and World States are implemented too
- Smart Objects logic are also available, and tested
- AI Sight Perception works
- The Finite State Machine logic has been implemented through the routine component
- The Custom Tree has also been implement but not used, in the future I'm thinking of using it for dialogue
- The Custom Blackboard also works but is now depreciated
- The Damage System is functional
- The Combat System is functional
- The Inventory system is functional, and the UI is functional too, it's just missing UI for item consumable and use case
- The Crafting System is functional and the minimal UI is done, it only needs to consume Inventory Items on craft
- The Hot Bar UI is functional, suffers from not updating from UI as items are added, keys do equip character with objects
- There is a routine system that works great with GOAP AI
- There is an in game clock that works well with other systems
- The UE5 built in system also works
---
Summary of the state of the project:
A lot of core systems have been built for designing AI which was a priority goal for my project, core gameplay systems have been started on and are now functional and things such as the layout design of the game have now been implemented. There is a "lose" and "win" condition.

---
How To Run :

As An Application : Navigate to the Build->Windows Folder of the project directory. Run the executable titled AI_Project.

Through the editor : In the Project Directory, run the .uproject "AI_Project", the compatible editor to run the project is Unreal Engine 5.4.4

---
Link to YT video : 
---
Visual Snippets of mechanics: 
<img src="VisualCOntent/AI_Demonsttrating_Attack_GOAP.gif" width="40" height="40" />
