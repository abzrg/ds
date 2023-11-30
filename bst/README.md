# Binary Search Tree Data structure

## Terminology around tree data structures

- Root: The most parent node.
- Height: The longest path from the root to the most child node.
- Binary tree: A tree in which has at most 2 children, at least 0 children.
- General tree: A tree with 0 or more children.
- Binary search tree: A tree in which has a specific ordering to the nodes and at most 2 children.
- Leaves: A node without children.
- Balanced: A tree is _perfectly_ balanced when any node's left and right children have the same height.
- Branching factor: The amount of children a tree has.

## Traversals

There are different ways in which you can visit the nodes of a tree. Runtime: O(n), we have to visit all n nodes.

```

                        7
                       / \
                      /   \
                     /     \
                    /       \
                   /         \
                  /           \
                 /             \
                23              3
               /  \            / \
              /    \          /   \
             /      \        /     \
            /        \      /       \
           /          \    /         \
          5            4  18         21

```

- pre order

    ```c
    visitNode()         // Do something with the node
    recurseLeft()
    recurseRight()
    ```

    ```
    7, 23, 5, 4, 3, 18, 21
    ```

- in order

    - use: in binary search tree, it will print values in a ascending fashion.

    ```c
    recurseLeft()
    visitNode()         // Do something with the node
    recurseRight()
    ```

    ```
    5, 23, 4, 7, 18, 3, 21
    ```

- post order

    - use: in deallocation of tree data structres

    ```c
    recurseLeft()
    recurseRight()
    visitNode()         // Do something with the node
    ```

    ```
    5, 4, 23, 18, 21, 3, 7
    ```

These types of traversals are known as _Depth First Search_.

- This is because the algorithm first traverses the whole tree depth (recurse left).
- The _stack_ data structure is implicitly being used for this type of traversal.
    - For example: dynamics of a post order traversal stack

    ```
                             [ 5  ]*   [ 4  ]*                       [ 18 ]*   [ 21 ]*
                   [ 23 ]    [ 23 ]    [ 23 ]    [ 23 ]*   [ 3  ]    [ 3  ]    [ 3  ]    [ 3  ]*
    x -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ] -> [ 7  ]* -> x


    *: operation on data
    ```

But there are another type of traversal: _Breadth First Search_

- In this traversal you visit one level of the tree at a time.

    ```
    ->                  7
                       / \
                      /   \
                     /     \
                    /       \
         -------------------------------
                  /           \
                 /             \
    ->          23      ->      3
               /  \            / \
              /    \          /   \
         --------------------------------
            /        \      /       \
           /          \    /         \
    ->    5     ->     4  18   ->    21
    ```

    ```
    7, 23, 3, 5, 4, 18, 21
    ```

- The queue data structure is implicitly being used for this type of traversal.

    ```
    x
    [ 7  ]* <- [ 23 ] <- [ 3  ]                      // children of 7 are enqueued and then operation on node 7 is done
    [ 23 ]* <- [ 3  ] <- [ 5  ] <- [ 4  ]            // children of 23 are enqueued, and then operation on 23 is done and then 23 is dequeued
    [ 3  ]* <- [ 5  ] <- [ 4  ] <- [ 18 ] <- [ 21 ]  // children of 3 are enqueued, and then operation on 3 is done and then 3 is dequeued
    [ 5  ]* <- [ 4  ] <- [ 18 ] <- [ 21 ]            // ...
    [ 4  ]* <- [ 18 ] <- [ 21 ]
    [ 18 ]* <- [ 21 ]
    [ 21 ]*
    x
    ```
