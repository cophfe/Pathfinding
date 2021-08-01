#pragma once
#include "PathfindingNode.h"
#include "NecessaryHeaders.h"
#include "Heap.h"

//values that can be used to calculate hexagon
constexpr auto HEX_RAD_TO_APOTHEM = 0.86602540378f; //approx 6/7, exact sqrt(3)/2;
constexpr auto HEX_APOTHEM_TO_RAD = 1.15470053838f;//approx 7/6, exact 2/sqrt(3);

class CollisionManager;
class Scene;
class GameObject;

class Pathfinder
{
public:
	//used for rendering walls
	static constexpr int WALL_HEIGHT = 200;
	static constexpr int WALL_THICKNESS = 200;
	static constexpr float LINE_THICKNESS = 8;

	Pathfinder(int amountX, int amountY, float nodeOffset, Vector2 startPosition = { 0 });
	~Pathfinder();

	PathfindingNode* getNodeFromPoint(float x, float y);
	PathfindingNode* getNodeFromPoint(const Vector2* position);
	PathfindingNode* getNodeFromIndex(int x, int y);
	PathfindingNode* getRandomUnblockedNode();
	//give pathfinder draw data
	void initDraw(const char* tilingTexture, float textureScale, Color wallColor);
	//draws the background
	void drawBack();
	//draws the bound walls
	void drawWalls();
#ifdef DRAW_DEBUG
	//draws debug information
	void drawDebug();
#endif

	//gets the estimated cost to get to from node to end
	int getHeuristic(PathfindingNode* node, PathfindingNode* end);
	//returns the cost and the path to get between 2 nodes
	int AStarPath(PathfindingNode* start, PathfindingNode* end, std::vector<Vector2>* finalPath);

	//generate wall gameObjects from node graph
	void generateWalls(CollisionManager* collision, const char* texture, Scene* scene);
	//generate physical bounds from graph
	void generateBoundsFromGraph(CollisionManager* collision, b2Body** boundsBodyPointer);

	//getters
	inline PathfindingNode*** getNodes() { return nodes; }
	inline int getSizeX() { return sizeX; }
	inline int getSizeY() { return sizeY; }
	inline float getNodeOffset() { return nodeOffset; }
	//calculate bounds
	Rectangle getBounds();

private:
	PathHeap openList;
	//bounds rectangle
	Rectangle bounds;
	//drawing info
	Rectangle srcRect;
	Rectangle destRect;
	Texture2D* backgroundTexture;
	Color wallColor;
	float texScale;
	//nodes data
	PathfindingNode*** nodes;
	bool* closedList;
	//top left corner of screen
	Vector2 startPosition;
	int sizeX, sizeY, length;
	float nodeOffset;
};

