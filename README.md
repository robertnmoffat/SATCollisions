# SATCollisions
Simple project to learn and try out SAT collision detection with OpenGL.

# Separated Axis Theorem explaination
SAT is a pretty easy form of collision detection that basically allows you to apply the concepts of a 
bounding box collision detection method to convex polygons which can be rotated and still have accurate detection.
In order to achieve this, the idea is to basically squash all vertices of the two objects onto a one dimensional 
plane and then use normal bounding box collision detection to see if the furthest points of each object overlap. 
These one dimensional planes must be created and projected onto from many different angles to ensure that there are 
collisions from every point of view.<br/>
The one dimensional vectors to be projected upon are the normals (90 degree vector) of each side of each of the 
two polygons which are being checked. After creating one normal you have to loop through every vertex of the two 
polygons and project them upon the normal. The projection can be thought of as if you were shining a light towards 
the normal from the other side of the polygons, or squashing the polygons into the normal until they are one dimensional. 
What we are calculating is the shadow which is cast upon the normal vector. This is done by taking the dot product 
of each vertex vector and the normal and then dividing by the magnitude of the normal. This will give you a single number 
representing that vertex' position on the normal. You then use these positions to compare the minimum and maximum 
of each polygon to see if there is overlap. Repeat the projections using a normal for each side of two two polygons, 
checking for overlap. If there is any normal which the points do not have an overlap, then there is no collision. If 
all checks have an overlap, then there is a collision.
