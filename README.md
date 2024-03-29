# C++ Final Project
<b>C++ Final Project</b>

This project is a final project of my C++ Programming 2 course (University of Bern, 2020). 

The goal is to create an app with a GUI and include an external library.

My app opens the user's camera and draws some "scary stuff" around. It's basically a nightmare simulator. The simulation begins with the user deciding how scared they feel. Then they simply sit back and relax. It is all fun and games, until a spider starts walking on the user's face or a ghost appears.

You have been warned...


*App preview*
![App screenshot - spider](res/spider-screenshot.png?raw=true "Spider")
![App screenshot - ghost](res/ghost-screenshot.png?raw=true "Ghost")


<b>Challenges and learning points</b>
- working with OpenCV - displaying the webcam output in my custom widget, not just OpenCV default imshow window
- user segmentation which would allow placing the ghost in the background - tried Haar cascades, HOG descriptor, Watershed segmenter
- not scaring myself with too creepy ghosts
- SW development is not a linear process and the amount of time you put in does not always equal the amount of cool working features you get
- you often try many different approaches only to end up going back to the very first thing you tried many many hours ago
