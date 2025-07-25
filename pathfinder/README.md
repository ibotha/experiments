# C Pathfinder

A one-file pathfinder using Dijkstra's algorithm in C.

## Aim

The goal of this experiment was to make something using C, and to intentionally do things that only C lets you do. That being said, this is meant to be one file where I just dip my toes so its nothing crazy.

## How it works

We construct a grid map with a size as determined by some #defines in the source. Randomly place some walls in the grid and place a Start and End.

With this map we then constuct a corrosponding grid which holds data for Dijkstra's algorithm.
1. Distance to this point
2. Direction to the previous node
3. Whether we have visited the node

We then in very brute force fashion loop over every square in the grid and find the closest un-visited node, then update the paths to the nearby squares and mark the current square as visited.

Keep doing the above until we have exhaused every square or reached the end. Then follow the previous nodes from the end back to the start drawing in our path.

```
----------------------
|O**#   #            |
|# *** #       #     |
|    *#              |
|#   **#             |
|     *******#       |
|       ##  *****    |
|   ##        # *    |
|#            # *    |
|  #       #    *#   |
|        #     #*#  #|
|               **   |
|                *   |
|                * ##|
|           #    *   |
|                *  #|
|  #             *## |
|     ## #       ****|
|    #         #  # *|
|        #          *|
|   #    #    #    #^|
----------------------
```

## Outcome

That was super fun, I love the deliberate thought that C requires, sometimes "best practice" helps us do something the best way and move on with more important things, sometimes it puts us on auto-pilot where we stop thinking about what we are actually writing.

I learned C as my first ever language, C is increadibly simple, and C lets you do *ANYTHING*. The combination of these is the perfect storm for deliberate thought, like how I felt when making the [Ray Tracer](https://github.com/WolfenGames/RT_WeThinkCode_) (Although this time I don't need to adhere to the draconian style guide that lead to those functions being completely unreadable) I love how it feels to develop like this.

## What would I do differently next time?

* I intend to come back to this using more complex tree representations of the paths to allow for optimisations and expanding the applicability beyond 2d grids.
* I plan to extend the error handling a bit and parameterize some more things so that I can re-run given seeds and sizes easily as a method of debugging paths.
* Perhaps I could play around with representing this data, visualisations into the algorithm as it is running, letting you step through and see what it is doing at each point.
* Intrumentation, I plan on putting together a little bit of intrumentation tooling to test the performance of my various projects at a later date.
