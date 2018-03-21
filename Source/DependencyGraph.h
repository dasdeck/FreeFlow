#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DependencyGraph {

    private:

    OwnedArray<DependencyWorker> workers;
    OwnedArray<DependencyNode> nodes;

    WaitableEvent doneEvent;

    DependencyQue que;

    public:

    DependencyGraph():
    que(this) {

    }

    void process() {
        this->que.enQue();
        this->doneEvent.wait();
    }
};


class DependencyNode {

    public:

    const Array<DependencyNode*>& getDependecies() const = 0;
    void process() const = 0;

};

class DependencyQue {

    private:
    
    Array<DependencyNode*> nodes;
    int numNodesToGo;
    CriticalSection lock;
    WaitableEvent nextEvent;
    DependencyGraph* graph;
    
    friend class DependencyWorker;

    public:

    DependencyQue(DependencyGraph* graph) {
        this->graph = graph;
    }

    void enQue() {
        this->nodes = this->graph->nodes;
        this->numNodesToGo = this->nodes.size();
        
        this->next();
    }

    void finishNode() {
        this->numNodesToGo--;
        this->next();
    }

    void next() {
        if (numNodesToGo) {
            this->nextEvent->signal();
        } else {
            this->graph->doneEvent->signal();
        }
    }

    DependencyNode* getNextFreeNode()const {

        ScopedLock sl(this->lock);

        if (this->nodes.size()) {
            for (DependencyNode* node : this->nodes) {

                Array<DependencyNode*> deps = node->getDependecies();

                if (deps.size()) {

                    for (DependencyNode* dependingNode : deps) {

                        if (!this->nodes.contains(dependingNode)) {
                            this->nodes.remove(node);
                            return node;
                        }

                    } 

                } else {
                    return node;
                }
            }
        }

        return nullptr;
    }

};



class DependencyWorker {

    private:

    DependencyGraph* graph;

    public:

    DependencyWorker(DependencyGraph* graph) {
        this->graph = graph;
    }

    void run() {
        while (true) {
            DependencyNode* node = this->graph->que.getNextFreeNode();
            if (node) {
                node->process();
                this->graph->que.finishNode();
            } else {
                this->graph->que.nextEvent->wait();
            }
        }
    }

}