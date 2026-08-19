# Bolide

A 3D racing game project developed in **C++** using **OpenGL**. The goal of the project is to create an environment where the player can control a Formula 1 car driving on a recreated **Monza** circuit.

The project is developed as a desktop application for Windows using Visual Studio.

## 🎮 Current Features

The current version of the project includes:

- 3D scene rendering using OpenGL,
- a recreated Monza circuit,
- a 3D Formula 1 car model,
- textures and materials,
- a surrounding skybox,
- directional lighting,
- shadow mapping,
- depth testing,
- object transparency using blending,
- car movement based on its current orientation,
- acceleration and braking,
- maximum speed limitation,
- friction and gradual deceleration,
- steering while driving,
- the ability to stop the car,
- several camera modes.

## 📷 Game Screenshots

### Track View

> **Screenshot placeholder:**
>
> `![Track view](images/gameplay_track.png)`

### Bolide View

> **Screenshot placeholder:**
>
> `![Bolide view](images/gameplay_bolide.png)`

### Driver / Cockpit View

> **Screenshot placeholder:**
>
> `![Driver view](images/gameplay_driver.png)`

### Additional Gameplay Screenshot

> **Screenshot placeholder:**
>
> `![Gameplay](images/gameplay.png)`

Screenshots can be added later to the `images/` directory.

## 🎥 Camera Modes

The camera can operate in several modes:

| Key | Mode |
|---|---|
| `0` | Free camera |
| `1` | Driver head view |
| `2` | Camera behind the bolide |
| `3` | Front view |

### Free Camera

In free camera mode, the camera can be moved around the scene. The viewing direction is controlled with the mouse while holding the right mouse button.

## 🏎️ Controls

| Key | Action |
|---|---|
| `↑` | Accelerate |
| `↓` | Brake / reverse |
| `←` | Turn left |
| `→` | Turn right |
| `P` | Stop the bolide |
| `ESC` | Exit the application |

The bolide movement is calculated relative to its current orientation, so accelerating moves the car in the direction in which it is facing.

## 🛠️ Technologies and Libraries

The project uses:

- **C++20**,
- **OpenGL** – 3D graphics rendering,
- **GLFW** – window, keyboard and mouse handling,
- **GLEW** – OpenGL extension handling,
- **GLM** – mathematical operations and 3D transformations,
- **Assimp** – 3D model importing,
- **SOIL2** – texture loading,
- **GLSL** – vertex and fragment shader programming.

The required libraries are included in the `Dependencies/` directory.

## 📁 Project Structure

```text
Bolide/
├── Bolide.slnx
├── Bolide/
│   ├── inc/                 # Header files for project classes and components
│   ├── src/                 # Application source code
│   │   └── main.cpp
│   ├── shaders_config/      # GLSL shaders
│   └── Bolide.vcxproj       # Visual Studio project configuration
├── Dependencies/            # External libraries
└── README.md
```

The main project components include classes responsible for the camera, shaders, 3D models, textures, skybox and shadows.

## ▶️ Building and Running

1. Clone the repository.
2. Open `Bolide.slnx` in Visual Studio.
3. Make sure the `x64` configuration is selected.
4. Build the project.
5. Run the application.

The project is configured for **C++20** and uses the libraries included in the `Dependencies/` directory.

## 🌍 3D Scene

The main scene consists of a **Monza** circuit model with a Formula 1 bolide placed on the track. Separate shaders are used for the main objects and the skybox. Shadows are generated in a separate rendering pass using a depth map.

## 🚧 Future Development

The next planned step is to **implement gravity and more advanced physics for the bolide**.

Gravity will be particularly important because the track is not completely flat and contains changes in elevation. Ultimately, the bolide's movement should take the actual shape and height of the track into account instead of simply moving across the XZ plane.

Further improvements to the physics and driving behavior are also planned to make the movement of the bolide more realistic.

## 📌 Project Status

The project is currently under development. The basic rendering system, 3D models, camera system and basic bolide movement have already been implemented. Further development is focused on expanding the physics system and improving the realism of driving.
