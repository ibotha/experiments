// The intent here is to re-familiarize myself with c/c++
// Don't get stuck in the nitty-gritty. Think, Choose, Move On
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define INT_SIZE_BYTES 4
typedef enum MapValue {
  GAP = 0,
  WALL,
  PATH,
  START,
  END
} MapValue;

typedef struct Map {
  MapValue* data;
  unsigned int width;
  unsigned int height;
} Map;

/* Create a map for us to navigate.

   The map's size will be determined at runtime so to simplify things we include the data
   in the allocation of the map. Since this is just one allocation the caller
   does not need to free the internal data, only the map itself.
 */
Map* blank_map(unsigned int width, unsigned int height) {
  const int data_size = width * height * sizeof(MapValue);
  void *block = malloc(sizeof(Map) + data_size);
  Map *map = (Map*)block;
  map->data = (MapValue*)(block + sizeof(Map));
  map->width = width;
  map->height = height;
  memset(map->data, 0, data_size);
  return map;
}

void print_map(Map* map) {
  // Allocate a string for the border...there is certainly a better way to do this...
  // TODO: this but better
  char* border = (char*)malloc(map->width + 4);
  memset(border, '-', map->width + 2);
  border[map->width + 2] = '\n';
  border[map->width + 3] = '\0';

  printf("%s", border);
  for(int y = 0; y < map->height; y++) {
    printf("|");
    for(int x = 0; x < map->width; x++) {
      MapValue value = map->data[y*map->width+x];
      switch (value) {
      case GAP:
	printf(" ");
	break;
      case WALL:
	printf("#");
	break;
      case PATH:
	printf("*");
	break;
      case START:
	printf("O");
	break;
      case END:
	printf("^");
	break;
      default:
	printf("?");
      }
    }
    printf("|\n");
  }
  printf("%s", border);

  free(border);
}

void fill_map(Map* map) {
  int last_x = -1, last_y = -1;
  int placed_start = 0;
  for(int y = 0; y < map->height; y++) {
    for(int x = 0; x < map->width; x++) {
      MapValue* value = map->data + (y*map->width+x);
      *value = (rand() % 10) == 0;
      if (*value == GAP) {
	if (!placed_start) {
	  *value = START;
	  placed_start = 1;
	}
	last_x = x;
	last_y = y;
      }
    }
  }
  map->data[last_y*map->width+last_x] = END;
}

typedef enum Direction {
  UP = 0,
  RIGHT,
  DOWN,
  LEFT,
  UNKNOWN
} Direction;

typedef struct NodeData {
  Direction previous;
  int dist;
  int visited;
} NodeData;

/*Search through the list of nodes to get the one with the smallest distance that we have not visited yet.
  Returns -1 if no node is found.
 */
int get_closest_node(NodeData* node_data, int size) {
  int smallest = 10101010; // Just some random really big number.
  int ret = -1;
  for (int i = 0; i < size; i++) {
    NodeData* value = node_data + i;
    if (!value->visited && value->dist < smallest) {
      smallest = value->dist;
      ret = i;
    }
  }
  return ret;
}

/* Find a path using Dijkstra's algorithm.
   The following project will include better data structures and upgrade to A*.
 */
void find_path(Map* map) {
  NodeData* node_data = (NodeData*)malloc(map->width * map->height * sizeof(NodeData));
  NodeData init_data;
  init_data.previous = UNKNOWN;
  // Algorithm specifies INFINITY but this should be more than any possible path.
  init_data.dist = map->width * map->height;
  init_data.visited = 0;
  NodeData wall_data = init_data;
  // Setting this to visited should mean we don't consider walls as valid paths.
  wall_data.visited = 1;
  for (int i = 0; i < (map->width * map->height); i++) {
    node_data[i] = map->data[i] == WALL ? wall_data : init_data;
    //Set start node dist to 0 to get things going.
    if (map->data[i] == START)
      node_data[i].dist = 0;
  }
  while (1) {
    int i = get_closest_node(node_data, map->width * map->height);
    if (i == -1) {
      break;
    }
    int x = i % map->width;
    int y = i / map->width;
    MapValue v = map->data[i];
    NodeData* n = node_data + i;
    if (v == END) {
      printf("End node distance: %i\n", n->dist);
      MapValue* cursor = map->data + i;
      while (*cursor != START) {
	if (*cursor != END)
	  *cursor = PATH;
	switch (n->previous) {
	case UP:
	  cursor -= map->width;
	  n -= map->width;
	  break;
	case DOWN:
	  cursor += map->width;
	  n += map->width;
	  break;
	case LEFT:
	  cursor -= 1;
	  n -= 1;
	  break;
	case RIGHT:
	  cursor += 1;
	  n += 1;
	  break;
	default:
	  printf("No Path!\n");
	  *cursor = START;
	}
      }
      break;
    }
    if (v == WALL) {
      printf("We are on a wall at %i %i", x, y);
    }
    int dist = n->dist + 1;
    n->visited = 1;
    if (y > 0) { // Check above
      NodeData* above = node_data + i - map->width;
      if (above->dist > dist) {
	above->dist = dist;
	above->previous = DOWN;
      }
    }
    if (y < (map->height - 1)) { // Check below
      NodeData* below = node_data + i + map->width;
      if (below->dist > dist) {
	below->dist = dist;
	below->previous = UP;
      }
    }
    if (x < (map->width - 1)) { // Check right
      NodeData* right = node_data + i + 1;
      if (right->dist > dist) {
	right->dist = dist;
	right->previous = LEFT;
      }
    }
    if (x > 0) { // Check left
      NodeData* left = node_data + i - 1;
      if (left->dist > dist) {
	left->dist = dist;
	left->previous = RIGHT;
      }
    }
  }

  free(node_data);
}

int main(int argc, char **argv) {
  int seed = time(0);
  printf("seed: %i\n", seed);
  srand(seed); 
  Map* map = blank_map(MAP_WIDTH, MAP_HEIGHT);

  fill_map(map);
  
  find_path(map);
  
  print_map(map);

  free(map);
}
