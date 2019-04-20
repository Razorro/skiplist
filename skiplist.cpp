#include "skiplist.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>
using namespace::std;


skiplist::skiplist()
{
	std::random_device rd;
	gen = new std::mt19937(rd());
	dis = new std::uniform_real_distribution<>(0.0, 1.0);

	head = new skiplistNode(maxLevel);
	level = 1;
}

skiplist::~skiplist()
{
	delete gen;
	delete dis;

	skiplistNode *temp = head;
	while(temp->forward[0])
	{
		skiplistNode *oldNode = temp;
		temp = temp->forward[0];
		delete oldNode;
		oldNode = nullptr;
	}
}

unsigned long skiplist::randomLevel()
{
	unsigned long ret = 1;
	while ((*dis)(*gen) < p && ret < maxLevel)
		ret += 1;
	return ret;
}


skiplistNode* skiplist::searchKey(long score, string key)
{
	skiplistNode *lookup = head;
	for(int i=level-1;i>=0;--i)
	{
		while(lookup->forward[i] && lookup->forward[i]->score_ < score)
			lookup = lookup->forward[i];
	}

	lookup = lookup->forward[0];
	if(lookup && lookup->ele_ == key && lookup->score_ == score)
		return lookup;
	else
		return nullptr;
}

void skiplist::insertNode(string key, long score)
{
	skiplistNode *updateNode[maxLevel];
	skiplistNode *temp = head;
	for (int i = level - 1; i >= 0; --i)
	{
		while (temp->forward[i] && temp->forward[i]->score_ < score)
			temp = temp->forward[i];
		updateNode[i] = temp;
	}

	temp = temp->forward[0];
	if (temp && temp->ele_ == key)		// find the same node, update the node's score
		temp->score_ = score;
	else
	{
		unsigned long genLevel = randomLevel();
		if (genLevel > level)
		{
			for (unsigned long i = level; i < genLevel; ++i)
				updateNode[i] = head;
			level = genLevel;
		}
		temp = new skiplistNode(level, key, score);
		for (unsigned long i = 0; i < level; ++i)
		{
			temp->forward[i] = updateNode[i]->forward[i];
			updateNode[i]->forward[i] = temp;
		}
	}
}

bool skiplist::deleteNode(string key, long score)
{
	skiplistNode *updateNode[maxLevel];
	skiplistNode *temp = head;
	for (int i = level-1; i>=0; --i)
	{
		while (temp->forward[i] && temp->forward[i]->score_ < score)
			temp = temp->forward[i];
		updateNode[i] = temp;
	}

	temp = temp->forward[0];
	if (temp && temp->score_ == score && temp->ele_ == key)
	{
		for (auto i = 0; i < level; ++i)
		{
			if (updateNode[i]->forward[i] != temp)
				break;
			updateNode[i]->forward[i] = temp->forward[i];
		}
		delete temp;

		// fit current max node's level
		while (level > 1 && head->forward[level - 1] == nullptr)
			level--;

		return true;
	}

	return false;
}

void skiplist::testRandomGen(int times)
{
	std::vector<double> coll;
	for (int i = 0; i < times; ++i)
		coll.push_back((*dis)(*gen));
	
	for (auto& elem : coll)
		std::cout << elem << ';';
}

void skiplist::testNodeOperation(int times, string testKey)
{
	srand(time(nullptr));
	vector<int> coll;
	for (int i = 0; i < times; ++i)
		coll.push_back(rand());

	for (int i = 0; i < times; ++i)
		insertNode(testKey+std::to_string(i), coll[i]);

	for (int i = 0; i < times; ++i)
		deleteNode(testKey + std::to_string(i), coll[i]);

}


skiplistNode::skiplistNode(int level, string ele, long score)
{
	ele_ = ele;
	score_ = score;
	
	if(level == 0)
		forward = nullptr;
	else if(level > 0)
	{
		forward = new skiplistNode*[level];
		for (int i = 0; i < level; ++i)
			forward[i] = nullptr;
	}
}

skiplistNode::~skiplistNode()
{
	delete [] forward;
}