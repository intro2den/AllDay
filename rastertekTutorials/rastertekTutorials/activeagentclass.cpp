#include "activeagentclass.h"

ActiveAgentClass::ActiveAgentClass(){
}

ActiveAgentClass::~ActiveAgentClass(){
}

void ActiveAgentClass::Move(int mapWidth, int mapHeight, int *costArray, int targetX, int targetY){
	bool validMove;

	Pathnode path;

	validMove = Search(mapWidth, mapHeight, costArray, targetX, targetY, &path);
}

bool ActiveAgentClass::Search(int mapWidth, int mapHeight, int *costArray, int targetX, int targetY, Pathnode *path){
	int currX, currY;

	getPosition(currX, currY);
	Pathnode queue[1024] = {};

	Pathnode temp;
	temp.tileX = currX;
	temp.tileY = currY;
	temp.cost = 0;
	temp.prev = nullptr;

	queue[0] = temp;

	while (queue[0].tileX != targetX || queue[0].tileY != targetY){
		int index = queue[0].tileX*mapHeight + queue[0].tileY;
		if (queue[0].tileX % 2 == 0){
			temp.cost = costArray[index - 1];
			temp.tileX = queue[0].tileX;
			temp.tileY = queue[0].tileY - 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + mapHeight - 1];
			temp.tileX = queue[0].tileX+1;
			temp.tileY = queue[0].tileY - 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + mapHeight];
			temp.tileX = queue[0].tileX + 1;
			temp.tileY = queue[0].tileY;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + 1];
			temp.tileX = queue[0].tileX;
			temp.tileY = queue[0].tileY+1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index - mapHeight];
			temp.tileX = queue[0].tileX - 1;
			temp.tileY = queue[0].tileY;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index - mapHeight - 1];
			temp.tileX = queue[0].tileX - 1;
			temp.tileY = queue[0].tileY - 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;
		}
		else {
			temp.cost = costArray[index - 1];
			temp.tileX = queue[0].tileX;
			temp.tileY = queue[0].tileY - 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + mapHeight];
			temp.tileX = queue[0].tileX + 1;
			temp.tileY = queue[0].tileY;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + mapHeight + 1];
			temp.tileX = queue[0].tileX + 1;
			temp.tileY = queue[0].tileY + 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index + 1];
			temp.tileX = queue[0].tileX;
			temp.tileY = queue[0].tileY + 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index - mapHeight + 1];
			temp.tileX = queue[0].tileX - 1;
			temp.tileY = queue[0].tileY + 1;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;

			temp.cost = costArray[index - mapHeight];
			temp.tileX = queue[0].tileX - 1;
			temp.tileY = queue[0].tileY;
			temp.prev = &queue[0];
			queue[sizeof(queue)-1] = temp;
		}

		
		/* indexing
		int index2 = index -1;
		int index3 = index + mapHeight - 1 (even) | index + mapHeight (odd);
		int index4 = index + mapHeight (even) | index + mapHeight+1 (odd);
		int index5 = index + 1;
		int index6 = index - mapHeight (even) | index - mapHeight+1 (odd);
		int index7 = index - mapHeight - 1 (even) | index - mapHeight (odd);

		odd true = queue[0].tileX % 2 == 1;
		even true = queue[0].tileX % 2 == 0;
		*/
	}
}