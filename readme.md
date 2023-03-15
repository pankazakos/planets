## Planets
### Description

This is an OpenGL app that renders Sun, Earth and Moon objects in a 3d scene. The Earth is moving around the Sun and rotating around itself, while the Moon is moving around the Earth. All rotations are counterclockwise. There is also lighting coming from the Sun which is reflected by the Earth and the Moon using [phong shading](https://en.wikipedia.org/wiki/Phong_shading). There is also a slight ambient lighting in the scene.


### Install dependencies (on ubuntu 22.04)
```
sudo apt install freeglut3-dev libglfw3 libglfw3 libglfw3-dev libglm-dev libassimp-dev assimp-utils libassimp-dev
```

### Glad configuration
Unzip Glad.zip in ./include directory
```
unzip ./include/Glad.zip -d ./include/
```



### compile and execute
```
make run
```

Alternatively, run the task from vscode

### Usage
W,A,S,D keys: movement in 3D scene.
mouse: camera movement in 3D scene.
scroll: zoom in and out.
Space key: toggle movement of Earth and Moon.

### LearnOpenGL
Credits to Joey de Vries from [LearnOpenGL](https://learnopengl.com/) for his great tutorial on OpenGL and providing the header files.

