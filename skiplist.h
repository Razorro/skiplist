#include <string>
#include <functional>
#include <random>
using std::string;


class skiplistNode
{
public:
	skiplistNode(int level, string ele="", long score=0);

	string ele_;
	long score_;
	skiplistNode **forward;

};


class skiplist
{
public:
	skiplist();
	~skiplist();
	unsigned long randomLevel();
	void insertNode(string key, long score);
	bool deleteNode(string key, long score);
	skiplistNode* searchKey(int score);

	static constexpr int maxLevel=64;
	static constexpr float p=0.25;
	unsigned long level;
	unsigned int length;

	skiplistNode *head;

	std::mt19937 *gen;
	std::uniform_real_distribution<> *dis;

	void testRandomGen(int times);
	void testNodeOperation(int times, string key);
};