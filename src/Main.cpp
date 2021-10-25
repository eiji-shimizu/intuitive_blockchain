#include "Ibc.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main()
{
    try {
        const int max = 40;
        std::vector<std::unique_ptr<Ibc::Node>> nodes;
        for (int i = 0; i < max; ++i) {
            bool isShift = false;
            if (i == 0) {
                isShift = true;
            }
            nodes.push_back(std::make_unique<Ibc::Node>(i, 0, max, isShift));
        }

        // ネットワーククラス初期化
        Ibc::Network::initialize(nodes);
        for (const std::unique_ptr<Ibc::Node> &p : nodes) {
            // 各店のスレッド開始
            p->start();
        }

        // 'q'が入力されれば終了
        char c = ' ';
        while (std::cin >> c) {
            if (c == 'q') {
                Ibc::Network::instance().setQuit();
                break;
            }
        }
        std::cout << "END TRY BLOCK OF main" << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "unexpected error." << std::endl;
    }
}
