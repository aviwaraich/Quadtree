# Quadtree
Quadtree is a data structure that is used to decompose an input image.

In short, a Quadtree  (Links to an external site.)is a data structure that is used to decompose an input image, in a hierarchical fashion (i.e. starting with big regions and breaking those down into smaller and smaller ones) in such a way that the final set of regions is made up of squares of uniform (or almost uniform) colour.

The key idea here is:

      If a large region is found that has a uniform colour, we can represent it as a large region with known coordinates, size, and colour, and this information requires much less memory to store than the values of the possibly many, many pixels that are contained within that region. If, on the other hand, the region doesn't have a uniform colour - well, then we split it into smaller sub-regions!

     We split only regions that do not have a nearly-uniform colour!

 The resulting image decomposition is useful for applications such as image compression, pattern/object recognition, image segmentation (breaking up an image into parts belonging to individual objects) and more.
 
 <img width="947" alt="Screen Shot 2022-07-18 at 12 50 25 AM" src="https://user-images.githubusercontent.com/56856829/179447186-36787da7-238f-4a60-9b71-ae3f1dabeab8.png">

 
 ![res](https://user-images.githubusercontent.com/56856829/179447169-743b3768-d327-43a8-aba9-bb2de08d2efe.gif)
