#include <iostream>
using namespace std;
int max(int a, int b) {
    return (a > b) ? a : b;
}
const int MAX_R = 5000;
const int MAX_L = 5000;
const int MAX_N = 100;
char gameMap[MAX_L+1][MAX_R];
int R, L, N;

char bag[100 * 100];
int bagSize = 0;

char inventory[MAX_N];
int front = 0, rear = 0;

bool isInventoryEmpty() {
    return front == rear;
}

void enqueue(char item) {
    inventory[rear++] = item;
}

char dequeue() {
    if (!isInventoryEmpty()) {
        return inventory[front++];
    }
    return '_'; 
}

// Function prototypes
void dig(int col);
void explodeBomb(int i, int col);
void useItem();
void pig();
void lucky(int col);
void Mag();
void printFinalState();
void flashlight();
int main() {
    cin >> R >> L >> N;
    
    // Read the initial state of the map
    for (int i = L; i > 0; i--) {
        for (int j = 0; j < R; ++j) {
            cin >> gameMap[i][j];
        }
    }

    // Process actions
    
    for (int i = 0; i < N; ++i) {
        string action;
        cin >> action;
        if (action == "DIG") {
            int col;
            cin >> col;
            dig(col);
        } else if (action == "USE") {
            useItem();
        }
    }
    // Print the final state
    
    printFinalState();
    return 0;
}

void dig(int col) {
    if(col < 0 || col >= R){
        return;
    }
    for(int i = L; i > 0; i--){
        if(gameMap[i][col] != '_'){
            char item = gameMap[i][col];
            gameMap[i][col] = '_';
            //cerr << "dig:" << item << endl;
            switch (item) {
                case 'D': // 鑽石
                case 'G': // 黃金
                    bag[bagSize++] = item;
                    break;
                case 'F': // 手電筒
                case 'M': // 磁鐵
                    enqueue(item);
                    break;
                case 'B': // 炸彈
                    explodeBomb(i, col);
                    break;
                case 'C': // 幸運三葉草
                    lucky(col);
                    break;
                case 'P': // 豬
                    pig();
                    break;
            }
            break;
        }
    }
    return;

}
void Mag() {
    for (int col = 0; col < R; ++col) {
        dig(col); 
    }
}
void explodeBomb(int i, int col){
    for(int ii = i-1; ii<=i+1; ii++){
        for(int j=col-1; j<=col+1; j++){
            if((ii > 0 && ii <= L) && (j >= 0 && j < R)){
                gameMap[ii][j] = '_';
            }
        }
    }
}
void lucky(int col) {
    char topItemType = '_';
    for (int i = bagSize - 1; i >= 0; --i) {
        if (bag[i] == 'G' || bag[i] == 'D') {
            topItemType = bag[i];
            break;
        }
    }

    if (topItemType == '_'){
        return;
    }
    int maxLevel = 0;
    for (int offset = -2; offset <= 2; ++offset) {
        int currentCol = col + offset;
        if (currentCol >= 0 && currentCol < R) {
            for (int i = L; i >= 1; --i) {
                if (gameMap[i][currentCol] != '_') {
                    maxLevel = max(maxLevel, i);
                    break;
                }
            }
        }
    }
    if(maxLevel == 0) return;

    for (int newLevel = L + 1; newLevel <= L + 3; ++newLevel) {
        if (newLevel <= MAX_L) {
            for (int j = 0; j < R; ++j) {
                gameMap[newLevel][j] = '_';
            }
        }
    }

    for (int offset = -2; offset <= 2; ++offset) {
        int currentCol = col + offset;
        if (currentCol >= 0 && currentCol < R) {
            for (int i = 1; i <= 3; ++i) {
                int newLevel = maxLevel + i;
                if (newLevel < MAX_L) { 
                    gameMap[newLevel][currentCol] = topItemType;
                    L = max(L, newLevel);
                }
            }
        }
    }
}
void useItem() {
    if (isInventoryEmpty()) {
        return;
    }
    char item = dequeue();
    //cerr << item << endl;
    switch (item) {
        case 'F':
            flashlight();
            break;
        case 'M': 
            Mag();
            break;
        default:
            break;
    }
}
void pig() {
    if(bag[bagSize-1] == 'D') return;
    int lastDiamondIndex = -1;
    for (int i = 0; i < bagSize; ++i) {
        if (bag[i] == 'D') {
            lastDiamondIndex = i;
        }
    }

    if (lastDiamondIndex != -1) {
        int newSize = lastDiamondIndex + 1;
        for (int i = lastDiamondIndex + 1; i < bagSize; ++i) {
            if (bag[i] != 'G') {
                bag[newSize++] = bag[i];
            }
        }
        bagSize = newSize;
    }
    else{
        bagSize = 0;
    }
}
void flashlight(){
    int maxlevel=0;
    for(int i=0;i<R;i++){
        for(int j=L;j>0;j--){
            if(gameMap[j][i] != '_'){
                if(j > maxlevel) maxlevel=j;
                break;
            }
        }
    }
    if(maxlevel == 0){
        cout << "MINE LEVEL:1\n";
        for (int j = 0; j < R; ++j) {
            cout << "_ ";
        }
        cout << endl;
    }
    else{
        cout << "MINE LEVEL:" << maxlevel << "\n";
        for (int j = 0; j < R; ++j) {
            char item = gameMap[maxlevel][j];
            if (item != '_') {
                cout << item << " ";
            } else {
                cout << "_ ";
            }
        }
        cout << "\n";
    }
}
void printFinalState() {
    cout << "FINAL BAG:\n";
    if (bagSize == 0) {
        cout << "\n";
    } 
    else {
        for (int i = 0; i < bagSize; i++) {
            cout << bag[i] << " ";
        }
        cout << "\n"; 
    }
    cout << "FINAL MAP:\n";
    bool flag=0;
    for(int i=L; i>0;i--){
        for(int j=0; j<R; j++){
            if(!flag && gameMap[i][j] != '_'){
                flag=1;
                break;
            }
        }
        if(flag){
            for(int k=0; k<R; k++){
                cout << gameMap[i][k] << " ";
            }
            cout << endl;
        }
    }
}
