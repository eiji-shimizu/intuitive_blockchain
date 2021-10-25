#include "Ibc.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main()
{
    const int max = 40;
    std::vector<std::unique_ptr<Ibc::Node>> nodes;
    for (int i = 0; i < max; ++i) {
        bool isShift = false;
        if (i == 0) {
            isShift = true;
        }
        nodes.push_back(std::make_unique<Ibc::Node>(i, 0, max, isShift));
    }
    for (const std::unique_ptr<Ibc::Node> &p : nodes) {
        p->start();
    }
}
