# Dungeons Inc.

![Example video game poster](https://i.imgur.com/0EQWKC2.png)

## Milestone 4: Implemented Features and Explanations

### Stability
<b>Prior Missed Features / Bugs</b><br />
- Mesh Collision fixed to now perform edge-edge checks (physics_system.cpp's mesh_collides_ground() and other bounding box helper functions).
- Switched parallax implementation from modifying position/velocity values to based on camera motion and vertex shaders.

<b>No Crashes, Glitches, or Unpredictable Behaviour</b><br />
- No noticeable issues.<br /><br />


### Playability
<b>10 Minutes of Gameplay</b><br />
- There are now 8 complete levels (1 tutorial and 7 other levels) with different adventurers and lengths (data/save_data/level_data.json).
- Level Select screen handles the additional levels (level_select_system.cpp's create_level_selector()).
- There are different adventurers and mobs appearing during combat (world_init.cpp's createMob() and combat_system.cpp's process_combat()).<br /><br />


### User Experience
<b>Comprehensive Tutorial</b><br />
- A replayable tutorial level, accompanied with dialogue following along with the player with directions.
- Dialogue system prompts player with appropriate advice during tutorial (handled in dialogue_system.cpp)

<b>Optimize User Interactions and REPORT it</b><br />
- Changes to user interactions based on our own playtest surveys and MTA feedback.
- Made pauses more apparent with pause_overlay.png.
- Removed clutter from top left by moving phase UI to center.
- Implemented entire player configuration and settings menu (menu_system.cpp).
- Players can exit a level in progress through menu.
- Players can toggle settings, such as music, sound effects, and difficulty, which are all saved and reloaded.
- Moved the dialogue box to make the combat room visible.
- Asked friends to play our game and provide feedback via Google Forms: https://docs.google.com/forms/d/1Ycar0aQK1xLpUmclE4KG0_5L-_dlq8wccHsWzUeVPvQ/viewanalytics<br /><br />


### Creative Features
<b>Parallax Scrolling Backgrounds</b><br />
- Level select menu's backgrounds create parallax effect on camera motion.
- Clicking a level button moves the camera, and the backgrounds move differently based on depth (level_select_system.cpp's handle_parallax() and step() and bgX.vs.glsl shaders).

<b>Advanced Story Elements</b><br />
- Added character-by-character text rendering and custom dialogue for Little Guy (handled in dialogue_system.cpp).
- Simple cutscenes with character animations and static images play before each level (handled in cutscene_system.cpp).
- Hidden dating sim??? (handled in cutscene_system.cpp).

<b>Numerous Sophisticated Integrated Assets</b><br />
- Original assets created to match existing aesthetic.
- Sprite sheets used in animation (lilguy sheet.png, running goblin.png).
- Various original textures for each card type and tier (under textures/card).
- Various background, overlay, and UI assets (such as dungeon_room.png, mob healthbar.png, red particle.png, victory.png, defeat.png, player_portrait.png, speech_bubble.png, pause_overlay.png,  play_overlay.png, etc).
- Additional textures that edit/transform other assets (UI assets under level_select and menu).

<b>Game Balance</b><br />
- Adjusted mob and adventurer strength based on playtesting, such that character stats and damage calculations follow a logarithmic formula (world_init.cpp's assign_stats() and combat_system.cpp's calculate_damage()).
- Added three customizeable difficulties that affect how challenging the AI movement will be (as_system.cpp's move_to_next_room()).
- Changed when cards are unlocked such that the more impactful ones are unlocked in later, more difficult levels (handled in data/save_data/level_data.json).<br /><br />


### ECS Diagram
https://drive.google.com/file/d/1mllToKNj2Yzveub-8fMDDXGoZFmF52me/view?usp=sharing<br /><br />


### Changes & Discrepancies of the Game Proposal Doc
- The overworld hub, narrative tracker, and different narrative endings were not implemented in this milestone.
- Instead, all sprites were updated to follow a more coherent style.
- Additionally, player configurations and settings were added to provide more accessibility and customization.
- These changes and their details can be found in the updated proposal document.<br /><br />



## Milestone 3: Implemented Features and Explanations

### Playability
<b>5 Minutes of Gameplay</b><br />
- There are now 5 complete levels (1 tutorial and 4 other levels), and can be won/lost (level_system.cpp's showVictoryScreen and showDefeatScreen).
- The levels now end gracefully, redirecting users to the level selection page (phase_system.cpp's next_phase() case: START)
- There is a dialogue system that enhances the gameplay experience (handled in dialogue_system.cpp)<br /><br />

### Robustness
<b>Memory Management</b><br />
- Used Dr. Memory to ensure that there are no memory leaks, even after extended playtime.
- Used Dr. Memory to pinpoint functions that hoard memory by analyzing all still-reachable allocation errors.

<b>Handle User Input</b><br />
- User inputs are limited to keyboard and mouse inputs (render_system.cpp's on_keyboard, on_mouse_click, on_mouse_move).
- Allowed inputs depend on the current_scene. Nothing should happen if other inputs occur.

<b>Real-time Gameplay</b><br />
- Game runs with no noticeable lag through extended playtime (multiple levels, repeating levels, etc).<br /><br />

### Stability
<b>Prior Missed Features / Bugs</b><br />
- The issue where adventurers HP would display negative HP numbers has been fixed.

<b>Consistent Game Resolution</b><br />
- The game runs at a default 1200 x 800 size on a default 1920 x 1080 resolution (aspect ratio calculated in common.hpp).
- Running the game at higher or lower resolutions will correctly resize the game window to keep a consistent aspect ratio (3 : 2) (handled in render_system.cpp's draw_text and drawTexturedMesh).

<b>No Crashes, Glitches, or Unpredictable Behaviour</b><br />
- No noticeable issues.<br /><br />

### Creative Features
<b>Particle System</b><br />
- During combat, particle systems create a splashing blood effect when creatures are damaged.
- Blood and particle systems are handled in animation_system.cpp's animate_blood() and create_blood_particle().
- Particles are rendered in render_system.cpp's draw_particles().

<b>Audio System</b><br />
- Sound effects added for certain actions, such as card draw and card merging.
- Sound effect assets in data/audio.
- Calling and loading sounds handled in audio_system.cpp.

<b>Reloadability</b><br />
- Game loads the cards, levels, and dialogue from json files (handled in load_level_data, load_deck_data, and load_dialogue_data).
- Game saves the user's level progression on new level win (handled in level_system.cpp's save_level_json).
- Subsequent game loads will have the level progression retained from previous playthroughs.

<b>Simple Parallax</b><br />
- There is basic parallax on the level select menu, consisting of four layers.
- Choosing a different level moves the background, with each depth moving differently.
- Parallax handled in level_select_system.cpp's handle_parallax() and step().<br /><br />

### ECS Diagram
https://drive.google.com/file/d/1mllToKNj2Yzveub-8fMDDXGoZFmF52me/view?usp=sharing<br /><br />

### Changes & Discrepancies of the Game Proposal Doc
- Implementing different card types, the overworld hub, and cutscene engine will be pushed to the next milestone.
- The narrative path tracker may or may not be included in the next milestone.
- Updates to the user interface were done instead of the above features.<br /><br />



## Milestone 2: Implemented Features and Explanations

### Improved Gameplay

<b>Game Logic</b><br />
- Applying a card to a room can change the pathfinding AI's determined path (handled in ai_system.cpp's move_to_next_room).
- The AI attempts to minimize the amount of damage taken by the adventurers / adventurer deaths when determining the next room to proceed with (handled in ai_system.cpp's move_to_next_room).

<b>Animation</b><br />
- Adventurers and mobs have simple bouncing idle animations done using sprite sheets (handled in world_system.cpp's updateAnimations and render_system.cpp's drawAnimatedSprite).
- Cards in the tutorial level have a glowing animation applied for clarity purposes (handled in render_system.cpp's drawTexturedMesh and shaders/glowing.fs.glsl).

<b>Assets</b><br /> 
- Card tiers were added to pave the way for card merging, which allows you to merge duplicate cards into stronger versions (handled in card_functions.cpp's update_card_tier and merge_cards).
- A room can now have multiple pathways to rooms in the next row, allowing for more variance in the pathfinding AI's decision making (handled in map_system.cpp's generate_rooms).
- An animated splash screen helps promote the product and draw in players (handled in intro_system.cpp).

<b>Mesh-Based Collision Detection</b><br /> 
- The splash screen contains the team name's letters, which are meshes (handled in intro_system.cpp).
- The letters fall from the top of the screen before colliding with the flat ground to produce a bouncing animation (handled in physics_system.cpp).

<b>Help</b><br /> 
- A tutorial level is available at the beginning of the game (called in world_system.cpp's init).
- There are various text boxes that provide explanations to the user about basic game mechanics and a general overview of a typical level (handled in phase_system.cpp's create_tutorial_ui and update_tutorial_ui).<br /><br />

### Playability

<b>2 Minutes of Non-Repetitive Gameplay</b><br />
- Each level (the tutorial and level 1) takes approximately one minute each to complete, handled in level_system.cpp. (initialize_levels, begin_level and reset_level)
- Each level has different adventurers and randomized cards, allowing for differences in gameplay.<br /><br />

### Stability

<b>Minimal Lag</b><br />
- No noticeable issues.

<b>Game Resolution and Aspect Ratio</b><br />
- The game runs at a default 1200 x 800 size on a default 1920 x 1080 resolution (aspect ratio calculated in common.hpp).
- Running the game at higher or lower resolutions will correctly resize the game window to keep a consistent aspect ratio (3 : 2) (handled in render_system.cpp's draw_text and drawTexturedMesh).

<b>No Crashes, Glitches, or Unpredictable Behaviour</b><br />
- No noticeable issues.<br /><br />

### Creative Features

<b>Simple Rendering Effects</b><br />
- Cards glow during the tutorial level for clarity purposes (handled in render_system.cpp's drawTexturedMesh and shaders/glowing.fs.glsl).<br />
- Cards and rooms glow when a card that can interact with them (either by merging or applying a card effect) is dragged over them (handled in render_system.cpp's on_mouse_click and world_system.cpp's handle_collisions).

<b>Simple Path Finding</b><br />
- Characters use depth-first search to choose a dungeon room to advance to.
- DFS is handled in map_system.cpp (dfs)<br /><br />

### ECS Diagram

https://drive.google.com/file/d/1mllToKNj2Yzveub-8fMDDXGoZFmF52me/view?usp=sharing
- Map no longer had Rooms compoenent. Map now has Map component. Room no has Room component for the rooms that they connect to.
- The level entity has been changed into the level system, with their own entities and components that control levels.
- Additions of new entities and components related to the cutscene system
- Small changes to the card system for tiers and merging capabilities<br /><br />

### Changes & Discrepancies of the Game Proposal Doc

- The core gameplay mechanics laid out in Milestone 2 have been implemented, with some changes. 
- Animation and cutscene engine worked will be started in Milestone 3.
- Additionally, implementing different card types and full level creation will also be pushed to subsequent milestones. 
- These changes and their details can be found in the updated proposal document.<br /><br />


## Milestone 1: Implemented Features and Explanations

### Rendering

<b>Textured Geometry</b><br />
- Game is loaded with backgrounds, rooms, UI windows, and character sprites - all using textures.
- Most rendering capabilities can be found in render_system.cpp (drawTexturedMesh).
- New textures have been added to the /data/textures folder and to texture_paths in render_system.hpp.

<b>Basic 2D Transformations</b><br />
- Cards and their text can be translated around the map with a mouse click and drag.
- Characters are automatically translated to the next room in their determined path.
- Transformations use the Transform struct, which takes position and camera into account (render_system.cpp's drawTexturedMesh and draw_text).

<b>Key-Frame/State Interpolation</b><br /> 
- Characters have a basic idle animation that involves them bobbing up and down.
- Updating animated entities occurs in world_system.cpp (updateAnimations).
- Rendering animated sprites handled in render_system.cpp (drawAnimatedSprite).

### Gameplay

<b>Keyboard/Mouse Control</b><br />
- Cards can be moved by clicking and dragging the mouse cursor. Camera controls move the camera view using arrow keys.
- Handling of the mouse click and drag occurs in render_system.cpp (on_mouse_click, on_mouse_move, and handle_card_drag).
- Handling of the camera controls occurs in render_system.cpp (on_keyboard).

<b>Random/Coded Action</b><br />
- Combat system calculates damage based on the characters' stats and any cards that have been applied to the room. Dungeon room paths are generated randomly.
- Dungeon room path generation is handled in maps_system.cpp (generate_rooms).
- Combat feature and damage calculations are handled in combat_system.cpp (process_combat and calculate_damage).

<b>Well-Defined Game-Space Boundaries</b><br />
- Cards and camera controls are bounded to the playing area / game window.
- Card bounding is handled in render_system.cpp (handle_card_drag).
- Camera bounding is handled in render_system.cpp (on_keyboard).

<b>Simple Collision Detection & Resolution</b><br />
- Cards snap into place when being clicked and dragged over a dungeon room for one second.
- Resolution of card-room collisions is in world_system.cpp (step).<br /><br />

### Stability
<b>Minimal Lag</b><br />
- No noticeable issues.

<b>No Crashes, Glitches, or Unpredictable Behaviour</b><br />
- No noticeable issues.<br /><br />

### Creative Features
<b>Basic Story Elements with Text Rendering & Text Sprites:</b><br />
- An introduction to the game's story appears at the beginning. Text rendering is done with FreeType.
- Lore textbox is handled in phase_system.cpp (create_lore_ui, create_lore_textbox, and update_lore_textbox).
- Initializing the font face occurs in render_system_init.cpp (initTextFonts).
- Rendering text is handled in render_system.cpp (draw_text).

<b>Camera Controls</b><br />
- Camera controls move the camera view using the arrow keys. This also handles the movement of other rendered textures that should move along with the camera as well.
- Camera controls and bounding are handled in render_system.cpp (on_keyboard).

<b>Simple Path Finding</b><br />
- Characters use depth-first search to choose a dungeon room to advance to.<br />
- DFS is handled in map_system.cpp (dfs)<br /><br />

### ECS Diagram

https://drive.google.com/file/d/1mllToKNj2Yzveub-8fMDDXGoZFmF52me/view?usp=sharing<br /><br />

### Changes & Discrepancies of the Game Proposal Doc

- The core gameplay mechanics laid out in Milestone 1 have been implemented, with some changes. 
- The basic creative feature "saving and loading states" will be pushed to the next milestone, and was replaced with "basic story elements" for Milestone 1.
- Additionally, implementing different card types and full level creation will also be pushed to subsequent milestones. 
- These changes and their details can be found in the updated proposal document.<br /><br />

## Citations

### Assets
- Round Phase UI: https://anokolisa.itch.io/sidescroller-pixelart-sprites-asset-pack-forest-16x16
- monogram Font: https://datagoblin.itch.io/monogram
- music.wav: https://www.fesliyanstudios.com/royalty-free-music/download/8-bit-adventure/2282
- Character and Weapon Sprites: https://0x72.itch.io/dungeontileset-ii
- Hand Area texture: https://www.bing.com/images/create?FORM=GENILP
- Level Selection Background: https://szadiart.itch.io/pixel-fantasy-caves
- Level Selection UI: https://mounirtohami.itch.io/pixel-art-gui-elements
- Icons: https://piiixl.itch.io/1-bit-icons-part-2
- Sound effects: https://mixkit.co/free-sound-effects/game/

### Code
- Fonts and Printing Strings
   - https://learnopengl.com/In-Practice/Text-Rendering
   - https://freetype.org/freetype2/docs/tutorial/step1.html
   - https://www.youtube.com/watch?v=qW_8Dyq2asc&t=410s&ab_channel=MichaelGrieco
- Misc.
   - https://stackoverflow.com/questions/3909784/how-do-i-find-a-particular-value-in-an-array-and-return-its-index
   - https://community.khronos.org/t/window-size-in-glsl/57179
