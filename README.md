# DCD Simulator (Digital Circuit Design)

DCD Simulator is a fast, interactive logic circuit designer and simulator built from scratch in C++20. It allows users to place logic gates, route wires dynamically, and test digital circuits in real-time on an infinite 2D canvas. 

## Screenshots

![Screenshot 1: Main Canvas](docs/screenshot1.png)
*Caption: Designing a basic logic circuit.*

![Screenshot 2: Radial Menu](docs/screenshot2.png)
*Caption: The paged radial menu for fast component selection.*

![Screenshot 3: Complex Circuit](docs/screenshot3.png)
*Caption: A complex circuit in action.*

![Screenshot 4: Save and Load](docs/screenshot4.png)
*Caption: The file manager for custom .dcd save files.*

*(Note: Replace the image paths in the docs folder with your actual screenshots)*

## Features

* **Real-time Simulation:** Physics and logic states are calculated and updated at 60 ticks per second.
* **Component Library:** Includes standard logic gates (AND, OR, NOT, NAND, NOR) and interactive I/O components (Buttons, Bulbs, Junctions).
* **Smart Wiring:** Draw wires dynamically. The engine uses a spatial garbage collector to automatically clean up orphaned junctions.
* **Paged Radial Menu:** Hold a hotkey to open a fast, split-category radial menu. Scroll the mouse wheel to flip through component pages instantly.
* **Infinite Canvas:** Pan with the mouse and zoom with the scroll wheel to build circuits of any size.
* **Custom Save Format:** Save and load your circuits using our proprietary .dcd file format.

## How to Build (Linux)

This project uses a "Smart" Makefile. You do not need to manually install dependencies like Raylib or ImGui; the Makefile will automatically download and build them for you.

### Requirements
* GCC compiler with C++20 module support (g++)
* Standard build tools (make, git)
* X11 and OpenGL development libraries

### Build Instructions
1. Clone the repository and navigate into the folder:
~~~bash
git clone https://github.com/YourUsername/DCDs.git
cd DCDs
~~~

2. Run the makefile:
~~~bash
make
~~~

3. Run the simulator:
~~~bash
./dcds
~~~

## Download & Play (Windows)

If you are on Windows, you do not need to compile the code from source.
1. Navigate to the **Releases** tab on this GitHub repository.
2. Download the latest dcds-windows.zip file.
3. Extract the folder and run dcds.exe.

## How to Use the Simulator

### Controls
* **Left Click:** Place components, lock in wires, toggle Buttons, or interact with UI elements.
* **Right Click & Drag:** Pan the camera around the infinite board.
* **Scroll Wheel:** Zoom the camera in and out.
* **Hold TAB:** Open the Tools menu (Select / Wire). Release to equip the highlighted tool.
* **Hold SPACE:** Open the Components menu. **Scroll the mouse wheel** while holding SPACE to cycle through different pages of components (Basic Gates, Extra Gates, I/O).
* **Delete Key:** Remove the currently selected component, wire, or junction.

### Saving and Loading
Use the top menu bar to open the File menu. Saving a file will automatically append the .dcd extension and place it in your system's Documents folder under DCDsimulator.

## License

This project is open-source and available under the MIT License.
