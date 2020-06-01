Planet collision algorithm:

If near planet:

1. Calculate angle between each planet side's normal and the camera position
2. Check collision with the planet side with an angle < 45 degrees
3. Vector from camera to planet center and find collision point with the colliding face
4. Find the nearest vertex on the face grid and get the height of that vertexlibiomp5md.lib

For #4: treat the planet side as a plane and check plane collision with the vector
find the plane with the shortest distance

either find the nearest vertices to the center
or
loop through all vertices on the plane