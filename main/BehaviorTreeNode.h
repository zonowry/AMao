#pragma once
#include <cstdio>
#include <iostream>
#include <vector>

#define Debug
enum NodeStatus
{
	e_status_running,
	e_status_success,
	e_status_failure,
};

class BehaviorTreeNode
{
public:
	virtual void Enter() = 0;
	virtual NodeStatus Tick() = 0;
	virtual void Exit() = 0;
};


//////////////////////////
//////     分支节点    //////
//////////////////////////
class BranchTreeNode :public BehaviorTreeNode
{
private:
	std::vector< BehaviorTreeNode*> children;
	int currentIndex = 0;
	bool isRunning = false;

public:
	BranchTreeNode(std::vector<BehaviorTreeNode*>& nodes) :children(nodes) {}
	// 通过 BehaviorTreeNode 继承
	virtual void Enter() override
	{
		// 非运行状态，从第一个子节点开始
		if (!isRunning) {
			this->currentIndex = 0;
		}
	}

	virtual NodeStatus Tick() override
	{
		if (this->currentIndex < this->children.size())
		{
			NodeStatus status = children[this->currentIndex]->Tick();
			switch (status)
			{
			case e_status_running:
				isRunning = true;
				return e_status_running;
				break;
			case e_status_success:
				break;
			case e_status_failure:
				break;
			default:
				break;
			}
		}
	}

	virtual void Exit() override;

};


class SelectorTreeNode :public BehaviorTreeNode
{
public:
	SelectorTreeNode() {}
	~SelectorTreeNode() {}

private:


	// 通过 BehaviorTreeNode 继承
	virtual void Enter() override
	{

	}

	virtual NodeStatus Tick() override;

	virtual void Exit() override;

};
