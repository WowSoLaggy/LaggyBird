# LaggyBird

![flock](/imgs/flock.png)

Simple app that simulates bird flocks using the distributed behavioral model.
Done following the paper: http://www.cs.toronto.edu/~dt/siggraph97-course/cwr87/

## Behavior
Each bird is described by position, orientation, speed vector and customizable FOV.

Three simple behaviors are available:
1. Collision avoidance - each bird tries to avoid collision with the closest birds
2. Direction matching - each bird tries to match own speed with the speed of all nearby neighbours
3. Flocking - each bird tries to keep closer to the center of the nearby flock

Each if these modes can be switched separately using hotkeys.

## Hotkeys
* Esc - exit app
* Space - pause
* F1 - show / hide tooltip
* ~ - show metadata (FOV, direction, flock center, collisions avoidance weight)
* 1 - switch collision avoidance mode
* 2 - switch direction matching mode
* 3 - switch flocking mode
* - - decrease FOV
* + - increase FOV
* 0 - reset FOV

## Used code
[LaggyDx](https://github.com/WowSoLaggy/LaggyDx) is used as a DirectX framework.

[LaggySdk](https://github.com/WowSoLaggy/LaggySdk) is used as an SDK framework.
