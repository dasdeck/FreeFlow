#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "DependencyGraph.h"



class DummyNode : public DependencyNode {

    private:
    
    Array<DependencyNode*> deps;
    String name;

    public:

    DummyNode(const String& name, const Array<DependencyNode*>& deps = Array<DependencyNode*>()) {
        this->name = name;
        this->deps = deps;
    }

    const Array<DependencyNode*>& getDependecies() const override {
        return this->deps;
    };

    void process() const override {};
}


class DependencyGraphTest: public UnitTest {

    public:

    void performTest (UnitTestRunner *runner) {

        DependencyGraph graph;

        DummyNode input("input");
        DummyNode chainA1("A1", {&input});
        DummyNode chainB1("B1", {&input});
        DummyNode chainB2("B2", {&chainB1});
        DummyNode master("master",{&chainA1, chainB2});


    }

};