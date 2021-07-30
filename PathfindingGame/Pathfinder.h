#pragma once
#include "PathfindingNode.h"
#include "NecessaryHeaders.h"
#include "Heap.h"
constexpr auto HEX_RAD_TO_APOTHEM = 0.86602540378f; //approx 6/7, exact sqrt(3)/2;
constexpr auto HEX_APOTHEM_TO_RAD = 1.15470053838f;//approx 7/6, exact 2/sqrt(3);

class CollisionManager;
class Scene;
class GameObject;

class Pathfinder
{
public:
	static constexpr int WALL_HEIGHT = 200;
	static constexpr int WALL_THICKNESS = 200;
	static constexpr float LINE_THICKNESS = 8;

	Pathfinder(int amountX, int amountY, float nodeOffset, Vector2 startPosition = { 0 });
	~Pathfinder();

	PathfindingNode* getNodeFromPoint(float x, float y);
	PathfindingNode* getNodeFromPoint(const Vector2* position);
	PathfindingNode* getNodeFromIndex(int x, int y);
	PathfindingNode* getRandomUnblockedNode();
	void initDraw(const char* tilingTexture, float textureScale, Color wallColor);
	void drawBack();
	void drawWalls();
#ifdef DRAW_DEBUG
	void drawDebug();
#endif

	int getHeuristic(PathfindingNode* node, PathfindingNode* end);
	int AStarPath(PathfindingNode* start, PathfindingNode* end, std::vector<Vector2>* finalPath);

	void generateWalls(CollisionManager* collision, const char* texture, Scene* scene);
	void generateBoundsFromGraph(CollisionManager* collision, b2Body** boundsBodyPointer);

	inline PathfindingNode*** getNodes() { return nodes; }
	inline int getSizeX() { return sizeX; }
	inline int getSizeY() { return sizeY; }
	inline float getNodeOffset() { return nodeOffset; }
	Rectangle getBounds();

private:
	PathHeap openList;
	bool* closedList;

	PathfindingNode*** nodes;

	Vector2 startPosition;
	int sizeX, sizeY, length;
	float nodeOffset;

	Texture2D* backgroundTexture;
	Rectangle srcRect;
	Rectangle destRect;
	float texScale;
	Color wallColor;

	Rectangle bounds;
};

