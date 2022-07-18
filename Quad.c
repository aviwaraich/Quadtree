#include "imgUtils.c"

/**
 * This is the structure we are going to use to store each individual node of 
 * the BST. Remember that each Quad corresponds to a rectangular area on the 
 * image:
 *
 *                (tx,ty)         w
 *                   x-------------------------
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                 h |          Quad          |
 *                   |    key = tx+(ty*sx)    |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   |                        |
 *                   -------------------------x
 *                                       (tx + w, ty + h)
 *
 */
typedef struct quad {
  int tx, ty;  // The (x,y) coordinates of the top-left pixel in the quad
  int w;       // How many pixels wide the quad is
  int h;       // How many pixels high the quad is

  int sx;  // Width of the original image, this is needed for the key.
           // This *MUST* be the same for all nodes in the BST

  int key;  // A unique identifier (remember we discussed BST nodes
            // should have unique keys to identify each node). The
            // key identifier here will be created as:
            //       key = tx + (ty * sx)
            // This means that only one quad can start at a specific
            // pixel.

  int wsplit;  // 1 if this quad is supposed to be split along the width
               // 0 if this quad is supposed to be split along the height
    struct quad *left;
    struct quad *right;
  /** 
   * TODO: Complete the definition of the Quad struct
   */

} Quad;

///////////////////////////////////////////////////////////////////////////////

Quad *new_Quad(int tx, int ty, int w, int h, int wsplit, int sx) {
  /**
   * This function creates and initializes a new Quad for a rectanglecstarting 
   * at (tx, ty) with a width 'w' and height 'h'. The width ofcthe image in 
   * which this rectangle exists is 'sx', use this to computecthe key as:
   *
   * 		key = tx + (ty * sx)
   *
   */
    //Make a space for a each node
    Quad *node = (Quad *)calloc(1,sizeof(Quad));
    //Saving all of the values in node
    node -> tx = tx;
    node -> ty = ty;
    node -> w = w;
    node -> h = h;
    node -> wsplit = wsplit;
    node -> sx = sx;
    node -> key = tx+(ty*sx);
    //left and right = NULL
    node -> left = NULL;
    node -> right = NULL;
    //returning the node
    return node;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_insert(Quad *root, Quad *new_node) {
  /**
   * This function inserts a new Quad node into the BST rooted atc'root'. The 
   * new_node must already be initialized with validcdata, and must have a 
   * unique key.
   */
    
    if(root == NULL)
    {
        //if root is null then return the node we want to add
        return new_node;
    }
    //if key is already in there then leave it
    if(new_node->key == root->key)
    {
        printf("Duplicate Quad (tx,ty,sx)=%d,%d, %d, was ignored\n",new_node->tx,new_node->ty,new_node->sx);
    }
    //If key is more the root key then go in right root
    else if(new_node->key > root->key)
    {
        root->right = BST_insert(root->right,new_node);
    }
    //If key is less the root key then go in left root
    else
    {
        root->left = BST_insert(root->left,new_node);
    }
    //return root when done
    return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_search(Quad *root, int tx, int ty) {
  /**
   * This function searches the BST for a Quad at the speficied position. If 
   * found, it must return a pointer to the quad that contains it.
   */
    //If roots comes to end just return NULL
    if(root == NULL)
    {
        return NULL;
    }
    //Save a key;
    int key = tx+(ty*root->sx);
    //If key matchs return this Node
    if(root->key == key)
    {
        return root;
    }
    //If key is more than root key then go right
    if (key > root->key)
    {
        return BST_search(root->right,tx,ty);
    }
    //Else key is less than root key then go left
    else
    {
        return BST_search(root->left,tx,ty);
    }
    //just if nothing is there return NULL
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////

Quad *find_successor(Quad *right_child) {
  /**
   * This function finds the successor of a Quad node by searching the right 
   * subtree for the node that is most to the left (that will be the node
   * with the smallest key in that subtree)
   */
    //If roots left is null return the root
    if (right_child->left == NULL)
    {
        return right_child;
    }
    // keep going left if it is not
    return find_successor(right_child->left);
        //Sure check if anything goes wrong return NULL
}

///////////////////////////////////////////////////////////////////////////////

Quad *BST_delete(Quad *root, int tx, int ty) {
  /**
   * Deletes from the BST a Quad at the specified position.
   */
      Quad *tmp = NULL; //Save a TEMP
    // If root is NULL return NULL
      if(root == NULL)
      {
          return NULL;
      }
    // Save a key
      int key = tx+(ty*root->sx);
    //If key == root's key
      if(root->key == key)
      {
          // Case 1: No Children
          if (root->left==NULL && root->right==NULL)
          {
              // free that one node and return
              free(root);
              return NULL;
          }
          // Case 2: 1 Children on left
          else if(root->right==NULL)
          {
              // Save children on left and return
              tmp = root->left;
              free(root);
              return tmp;
          }
          // Case 3: 1 Children on right
          else if(root->left==NULL)
          {
              // Save children on right and return
              tmp = root->right;
              free(root);
              return tmp;
          }
          // Case 4: 2 Children on right and Left
          else
          {
              //Find the left most node on right node
              tmp = find_successor(root->right);
              //Chnage values of the node we want to delete's values to the left most node on right node
              root->tx = tmp->tx;
              root->ty = tmp->ty;
              root->w = tmp->w;
              root->h = tmp->h;
              root->wsplit = tmp->wsplit;
              root->sx = tmp->sx;
              root->key = tmp->key;
              //Go to the right node and delete that left most node
              root->right = BST_delete(root->right,tmp->tx, tmp->ty);
              //return the same node they gave with value changed
              return root;
          }
      }
    //If root's key is less than the key then go the right
      if (root->key < key)
      {
          root->right = BST_delete(root->right,tx,ty);
      }
    //If root's key is more than the key then go the left
      else
      {
          root->left = BST_delete(root->left,tx,ty);
      }
    // return root after root is done
    return root;
}

///////////////////////////////////////////////////////////////////////////////

Quad *delete_BST(Quad *root) {
  /**
   * This function deletes the BST and frees all memory used for nodes in it.
   */
    // If root is NULL then return NULL
    if (root == NULL)
    {
        return NULL;
    }
    // Do Post Order traversel
    delete_BST(root->left);
    delete_BST(root->right);
    // Find the root with no children delete it and resurive back
    free(root);
    // We need a empty root
    return NULL;;
}

///////////////////////////////////////////////////////////////////////////////

void BST_inorder(Quad *root, int depth) {
  /**
   * This function performs an in-order traversal of the BST and prints out the
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   */
    // Start depth + 1
    depth = depth + 1;
    // If root is NULL then return
    if (root == NULL)
    {
        return;
    }
    //Go to left most root
    BST_inorder(root->left,depth);
    //Print that root with depth-1 because depth start with 0 not 1
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",depth-1,root->key,root->tx,root->ty,root->w,root->h,root->wsplit);
    //Go to right most root
    BST_inorder(root->right,depth);
}

///////////////////////////////////////////////////////////////////////////////

void BST_preorder(Quad *root, int depth) {
  /**
   * This function performs a pre-order traversal of the BST and prints out the 
   * information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   *
   */
    // Start depth + 1
     depth = depth + 1;
    // If root is NULL then return
    if (root == NULL)
    {
        return;
    }
    //Print that root with depth-1 because depth start with 0 not 1
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",depth-1,root->key,root->tx,root->ty,root->w,root->h,root->wsplit);
    //Go to left most root
    BST_inorder(root->left,depth);
    //Go to right most root
    BST_inorder(root->right,depth);
}

///////////////////////////////////////////////////////////////////////////////

void BST_postorder(Quad *root, int depth) {
  /**
   * This function performs a post-order traversal of the BST and prints out 
   * the information for each Quad using this exactly this print statement:
   *
   *  printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",...)
   */
    // Start depth + 1
    depth = depth + 1;
    // If root is NULL then return
    if (root == NULL)
    {
        return;
    }
    //Go to left most root
    BST_inorder(root->left,depth);
    //Go to right most root
    BST_inorder(root->right,depth);
    //Print that root with depth-1 because depth start with 0 not 1
    printf("Depth=%d, key=%d, tx:ty (%d:%d), w=%d, h=%d, wsplit=%d\n",depth-1,root->key,root->tx,root->ty,root->w,root->h,root->wsplit);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int get_colour(Image *im, Quad *q) {
  /**
   * Given an image 'im' and a Quad 'q', get the colour we should be assigning
   * to the pixels that are in it, and return it. For the sake of this 
   * assignment, we will say this is *average* colour of all the pixels in 
   * the quad.
   */
    //Quad start with colour 0
      int colour = 0;
    // Go throught the whole image x cordinate
      for (int i=q->tx; i<(q->w+q->tx); i=i+1)
      {
          // Go throught the whole image y cordinate
          for (int j=q->ty; j<(q->h+q->ty); j=j+1)
          {
              // Keep adding the colour
              colour = colour + im->data[i + j * q->sx];
          }
      }
    //Return the average of the colour by divinding by width and height
    return colour/(q->w*q->h);
}

///////////////////////////////////////////////////////////////////////////////

int similar(Image *im, Quad *q, int threshold) {
  /**
   * Given an image 'im', check if the colours in the area corresponding to the 
   * Quad 'q' are all similar. If not, we will have to split it. For the 
   * purpose of this assigment, we say the colours in a Quad are similar if
   *
   *          maxCol - minCol <= threshold
   *
   * where maxCol and minCol are the maximum and minimum values respectively
   * of the pixel colours in the Quad. The threshold is a parameter. This
   * function should return a 0 if the pixels are not similar enough and the
   * Quad needs to be split, and 1 otherwise.
   */
    //Start Max colour with first one
      int maxCol = im->data[q->tx + q->ty * q->sx];
    //Start Min colour with first one
      int minCol = im->data[q->tx + q->ty * q->sx];
    //Go through the whole image
      for (int i=q->tx; i<(q->w+q->tx); i=i+1)
      {
          for (int j=q->ty; j<(q->h+q->ty); j=j+1)
          {
              // If there is a new max save that
              if(im->data[i + j * q->sx] > maxCol)
              {
                  maxCol = im->data[i + j * q->sx];
              }
              // If there is a new min save that
              if(im->data[i + j * q->sx] < minCol)
              {
                  minCol = im->data[i + j * q->sx];
              }
          }
      }
    // If maxCol - minCol <= threshold means no need to split return 1
      if (maxCol - minCol <= threshold)
      {
          return 1;
      }
    //Else return 0 means we need to split
    return 0;
}

///////////////////////////////////////////////////////////////////////////////


Quad *split_tree(Image *im, Quad *root, int threshold) {
  /**
   * This function traverses the BST, and for each existing Quad, checks if 
   * the pixels in the quad are of a similarcolour using the similar() function 
   * and the given threshold. If they are not, then the Quad needs to be split 
   * into 2 Quads(which will hopefully have pixels that are more similar to 
   * each other).
   *
   * To do this, first we need to decide in which direction we are going to 
   * split the Quad. For this, we will use the 'wsplit'field.
   *
   *    - If wsplit = 1, then we split it along the width (ie, we will now have 
   *                     2 quads with the same heightand half the width as the 
   *                     original one)
   *
   *    - If wsplit = 0, then we split along the height.
   *
   * -------------------------------------------------------------------------
   *
   * For example, if our Quad had the following values:
   *    (tx:ty = 0:0     w = 512,   h = 512,  wsplit = 1) ---> A
   *
   *                (0,0)
   *                   x-------------------------
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |           A            |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   |                        |
   *                   -------------------------x
   *                                         (512, 512)        
   *                
   *                                     * this pixel is not IN the image, just
   *                                       represents the 'corner'. The bottom
   *                                       right pixel, as always, is (511,511)
   * 
   * it would be split along the width, and the resulting two Quads
   * we would get would be as follows:
   *
   *     (tx:ty =  0 :0     w = 256,   h = 512,  wsplit = 0) ---> B
   *     (tx:ty = 256:0     w = 256,   h = 512,  wsplit = 0) ---> C
   *
   *
   *                (0,0)       (256, 0)
   *                   x-----------x-------------
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |     B     |      C     |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   |           |            |
   *                   -------------------------x
   *                                         (512, 512)
   *
   *   - Note that we want to always split it in exactly half, but if the
   *     width/height is an odd number then round down.
   *
   *   - Further note that 'wsplit' on both of these has now been set to 0.
   *     If they were split again, the resulting Quads would have wsplit = 1.
   *
   * --------------------------------------------------------------------------
   *
   * Expected result: The BST will have at most twice as many Quads
   *                  as before, depending on how many of them needed to be 
   *                  split.
   */
    // root is null return the root
    if (root == NULL)
    {
        return root;
    }
    // Doing Post Order Traversel
    split_tree(im,root->left,threshold);
    split_tree(im,root->right,threshold);
    // If similar == 0  means that that quad needs to be split
    if(similar(im,root,threshold) == 0)
    {
        // If wsplit == 1 cut from width
        if (root -> wsplit == 1)
        {
            // Change the old quad
            root->w = (root->w)/2; //cut width
            root->wsplit = 0; //Next split will be the hight
            // Insert a new quad
            BST_insert(root,new_Quad(root->w+root->tx,root->ty,root->w,root->h,root->wsplit,root->sx));
        }
        // If wsplit == 0 cut from hight
        else
        {
            // Change the old quad
            root->h = (root->h)/2; //cut height
            root->wsplit = 1; //Next split will be the width
            // Insert a new quad
            BST_insert(root,new_Quad(root->tx,root->h+root->ty,root->w,root->h,root->wsplit,root->sx));
        }
    }
    // return the root when done
    return root;
}

///////////////////////////////////////////////////////////////////////////////

void drawOutline(Image *im, Quad *root, unsigned char col) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each quad 
   * and draw an outline for it. The outline consists of the outermost pixels 
   * of the Quad (ie, the top and bottom rows, and the leftmost and rightmost 
   * columns).
   */
    // If root is NULL then return
    if (root == NULL)
    {
        return;
    }
    // Doing Post Order Traverael
    drawOutline(im,root->left,col);
    drawOutline(im,root->right,col);
    //Go through the while quad on x and y axis
    for (int i=root->tx; i<(root->w+root->tx); i=i+1)
    {
      for (int j=root->ty; j<(root->h+root->ty); j=j+1)
      {
          if(i == root->tx || j == root->ty || i == root->w+root->tx-1 || j == root->h+root->ty-1) // This is rhe Edge of the Quad
          {
              im->data[i + j * root->sx] = col; // Chnage the Edge Colour of the Quad
          }
      }
    }
}

///////////////////////////////////////////////////////////////////////////////

void save_Quad(Image *im, Quad *root) {
  /**
   * Given an image 'im' and a BST rooted at 'root', traverse through each 
   * quad, and set all the pixels in the corresponding area to the expected 
   * colour of the quad computed by function get_colour().
   *
   *
   */
    // If root is NULL then return
    if (root == NULL)
    {
        return;
    }
    // Doing Post Order Traverael
    save_Quad(im,root->left);
    save_Quad(im,root->right);
    //Getting Quads Average Colour
    int colour = get_colour(im,root);
    //Go through the while quad on x and y axis
    for (int i=root->tx; i<(root->w+root->tx); i=i+1)
    {
      for (int j=root->ty; j<(root->h+root->ty); j=j+1)
      {
          im->data[i + j * root->sx] = colour; // Chnage whole quads colour to the average colour.
      }
    }
}

///////////////////////////////////////////////////////////////////////////////
