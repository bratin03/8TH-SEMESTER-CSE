### How to check if the vertices of a polygon are in clockwise order or not?
-> We can sum over (x2-x1)(y2+y1) for each edge of the polygon. If the sum is positive, the vertices are in clockwise order, otherwise they are in counter-clockwise order.

### How to check if an angle is convex or concave?
-> If the cross product of the vectors from the vertex to the two adjacent vertices is positive, the angle is convex, otherwise it is concave.

### Checking if a polygon is monotonic with respect to any line
-> For concave angles, lines in some particular directions only will have the property that the polygon is monotonic with respect to them. We need to take intersection of those angles and check if the polygon is monotonic with respect to any of the lines.