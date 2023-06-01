#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Page {
    int id;
    bool referenced;
};

int main() {
    int numFrames;
    cout << "Digite o número de frames: ";
    cin >> numFrames;

    vector<Page> pageTable;
    vector<int> frameList(numFrames, -1);  // -1 representa um frame vazio

    int pageFaults = 0;
    int currentIndex = 0;

    int numReferences;
    cout << "Digite o número de referências de página: ";
    cin >> numReferences;

    cout << "Digite as referências de página: ";
    for (int i = 0; i < numReferences; i++) {
        int pageId;
        cin >> pageId;

        auto it = find_if(pageTable.begin(), pageTable.end(), [pageId](const Page& p) {
            return p.id == pageId;
        });

        if (it != pageTable.end()) {
            // Página encontrada na tabela
            it->referenced = true;
            frameList[it->id] = 1;
        } else {
            // Página não encontrada na tabela
            pageFaults++;

            while (frameList[currentIndex] == 1) {
                frameList[currentIndex] = 0;
                currentIndex = (currentIndex + 1) % numFrames;
            }

            if (frameList[currentIndex] != -1) {
                // A página atual do frame será substituída
                auto pageIterator = find_if(pageTable.begin(), pageTable.end(), [currentIndex](const Page& p) {
                    return p.id == currentIndex;
                });
                pageIterator->referenced = false;
            }

            // Atualiza o frame vazio com a nova página
            frameList[currentIndex] = 1;

            Page newPage;
            newPage.id = pageId;
            newPage.referenced = true;
            pageTable.push_back(newPage);

            currentIndex = (currentIndex + 1) % numFrames;
        }
    }

    cout << "Número de page faults: " << pageFaults << endl;

    return 0;
}
