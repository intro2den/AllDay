#include "activeagentclass.h"

ActiveAgentClass::ActiveAgentClass(){
}

ActiveAgentClass::~ActiveAgentClass(){
}

void ActiveAgentClass::Move(int mapWidth, int mapHeight, int *costArray, int targetX, int targetY){
	bool validMove;

	Pathnode path;

	validMove = Search(mapWidth, mapHeight, costArray, targetX, targetY, &path);
	if (validMove){
		setPosition(targetX, targetY);
	}
}

bool ActiveAgentClass::compare(Pathnode* x, Pathnode* y){
	return x->cost < y->cost;
}

bool ActiveAgentClass::Search(int mapWidth, int mapHeight, int *costArray, int targetX, int targetY, Pathnode *path){
	std::list <Pathnode*> queue;
	std::list <Pathnode*> deleteList;

    int x,y;
    getPosition(&x,&y);
    bool success = false;

	//Create first node of agents current location
	Pathnode* temp = new Pathnode;
	temp->tileX = x;
	temp->tileY = y;
	temp->cost = 0;
	temp->in = false;
	temp->prev = NULL;

	queue.push_back(temp);

    //search loop
	while (!queue.empty()){
		//found target hex
		if (queue.front()->tileX == targetX && queue.front()->tileY == targetY){
			path = queue.front();
			temp = path;
			//go through path to marking every node in it
			while (temp != NULL){
				temp->in = true;
				temp = temp->prev;
			}
			success = true;
			break;
		}

		int index = queue.front()->tileX*mapHeight + queue.front()->tileY;
		//if tile in even column
		if (queue.front()->tileX % 2 == 0){
            //add the neighbor six hexs to the queue
            //Note need to add if statements to determine if hex is a valid neighbor
			Pathnode* temp = new Pathnode;

			temp->cost = costArray[index - 1];
			temp->tileX = queue.front()->tileX;
			temp->tileY = queue.front()->tileY - 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + mapHeight - 1];
			temp->tileX = queue.front()->tileX + 1;
			temp->tileY = queue.front()->tileY - 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + mapHeight];
			temp->tileX = queue.front()->tileX + 1;
			temp->tileY = queue.front()->tileY;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + 1];
			temp->tileX = queue.front()->tileX;
			temp->tileY = queue.front()->tileY + 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index - mapHeight];
			temp->tileX = queue.front()->tileX - 1;
			temp->tileY = queue.front()->tileY;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index - mapHeight - 1];
			temp->tileX = queue.front()->tileX - 1;
			temp->tileY = queue.front()->tileY - 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;
		}
		//tile is in odd column
		else {
            //add the neighbor six hexs to the queue
            //Note need to add if statements to determine if hex is a valid neighbor
			temp->cost = costArray[index - 1];
			temp->tileX = queue.front()->tileX;
			temp->tileY = queue.front()->tileY - 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + mapHeight];
			temp->tileX = queue.front()->tileX + 1;
			temp->tileY = queue.front()->tileY;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + mapHeight + 1];
			temp->tileX = queue.front()->tileX + 1;
			temp->tileY = queue.front()->tileY + 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index + 1];
			temp->tileX = queue.front()->tileX;
			temp->tileY = queue.front()->tileY + 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index - mapHeight + 1];
			temp->tileX = queue.front()->tileX - 1;
			temp->tileY = queue.front()->tileY + 1;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
			temp = new Pathnode;

			temp->cost = costArray[index - mapHeight];
			temp->tileX = queue.front()->tileX - 1;
			temp->tileY = queue.front()->tileY;
			temp->prev = queue.front();
			temp->in = false;
			queue.push_back(temp);
			deleteList.push_back(temp);
		}
		queue.pop_front();
		queue.sort(compare);
	}
    //clear all nodes not in path
	std::list<Pathnode*>::iterator it;
	for (it = deleteList.begin(); it != deleteList.end(); ++it){
		if ((*it)->in == false){
			free(*it);
		}
	}

    return success;

}
