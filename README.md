# Silent Mansion
Made in Unreal Engine 5.4.4

Silent Mansion is my take on the Escape brief. It is a survival horror game taking place in an abandoned haunted mansion where the main character is inside and must eventually defeat the malevolent spirit holding them captive to escape, while also fighting against other spirits in the mansion.

It draws inspiration from games like Silent Hill, Luigi's mansion, Persona & Faith: The Unholy Trinity.

It's key features are the encountered AI the player can face as they have varied designs, the gameplay and combat system. 

---
Some technical Interesting Elements In my Game Are:
- A GOAP Planner (made in C++) that is exposed to the Unreal Editor
- GOAP World State, Actions and Goals (all made in C++) that are also designed to be designer friendly and also designed in editor
- Finite State Machines (C++)
- Behavior Trees (C++)
- Damage and Attack Systems (C++)
- Smart Objects(C++)
- A Custom Blackboard (C++)
- UE5 built in perception system (logic using this was in C++, configured in blueprints)
---
The Current State Of Play/What has been Implemented So Far :
- The GOAP Planner for the most part is functional, it works
- Actions, Goals and World States are implemented too
- Smart Objects logic are also available, and tested
- AI Sight Perception works
- The Finite State Machine logic has been implemented but not used
- The Custom Tree has also been implement but not used
- The Custom Blackboard also works and is used
- The Damage System has been started on but isn't functional yet
- The Combat System has been started on but not functional yet
- The UE5 built in system also works
---
Summary of the state of the project:
A lot of core systems have been built for designing AI which was a priority goal for my project, core gameplay systems have been started on but are not functional and things such as the layout design of the game has not yet been implemented. There is a "lose" condition though.

---
How To Run :

As An Application : Navigate to the Build->Windows Folder of the project directory. Run the executable titled AI_Project.

Through the editor : In the Project Directory, run the .uproject "AI_Project", the compatible editor to run the project is Unreal Engine 5.4.4

---
<video width="640" height="360" controls>
  <source src="Small Demonstration Video.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>
