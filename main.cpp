#include <iostream>
#include <string>
#include <queue>
using namespace std;


//What I added
//Song class
class SongClass {
    private:
        static int nextID;

    public:
    
        string title;
        string artist;
        int ID;
        
        SongClass() : title(""), artist("") {
            ID = nextID++;
        }   // DEFAULT CONSTRUCTOR ✔
    
        SongClass (string t, string a )
            : title(t), artist(a) {
                ID = nextID++;
            }
};
int SongClass::nextID = 1;


//
// SONG struct 
struct Song {
    SongClass song; //data is of type Song class now
    Song* next;
};

Song* head = nullptr;

// Helper for safe line input
void getLineSafe(string &s) {
    getline(cin, s);
}
//BST Structure and Algorithm
struct BSTNode {
    Song* songPtr;       
    BSTNode* left;
    BSTNode* right;

    BSTNode(Song* s) : songPtr(s), left(nullptr), right(nullptr) {}
};

class SongBST {
public:
    BSTNode* root;

    SongBST() : root(nullptr) {}

    void insert(Song* songNode) {
        root = insertHelper(root, songNode);
    }

    Song* search(int id) {
        BSTNode* result = searchHelper(root, id);
        return result ? result->songPtr : nullptr;
    }

    void inOrder() {
        inOrderHelper(root);
    }

private:
    BSTNode* insertHelper(BSTNode* node, Song* songNode) {
        if (node == nullptr)
            return new BSTNode(songNode);

        if (songNode->song.ID < node->songPtr->song.ID)
            node->left = insertHelper(node->left, songNode);
        else if (songNode->song.ID > node->songPtr->song.ID)
            node->right = insertHelper(node->right, songNode);

        return node;
    }

    BSTNode* searchHelper(BSTNode* node, int id) {
        if (!node || node->songPtr->song.ID == id)
            return node;

        if (id < node->songPtr->song.ID)
            return searchHelper(node->left, id);

        return searchHelper(node->right, id);
    }

    void inOrderHelper(BSTNode* node) {
        if (!node) return;
        inOrderHelper(node->left);

        cout << node->songPtr->song.ID << " | "
             << node->songPtr->song.title << " - "
             << node->songPtr->song.artist << endl;

        inOrderHelper(node->right);
    }
};


SongBST bst;  

// PLAYLIST FUNCTIONS
void addSong(string title, string artist) {
    Song* newSong = new Song;
    newSong -> song.title = title;
    newSong -> song.artist = artist;

    if (head == nullptr) {
        head = newSong;
    } else {
        Song* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = newSong;
    }
    bst.insert(newSong);
}

void addSongUser() {
    string title, artist;
    int ID;
    cout << "\nEnter song title: ";
    getLineSafe(title);
    cout << "Enter artist name: ";
    getLineSafe(artist);


    addSong(title, artist);
    cout << "\nSong added successfully.\n";
}

void displaySongs() {
    if (head == nullptr) {
        cout << "\nPlaylist is empty.\n";
        return;
    }

    cout << "\nYour Playlist:\n";
    Song* temp = head;
    int count = 1;
    while (temp != nullptr) {
        cout << count++ << ". " << temp->song.title << " - " << temp->song.artist << endl;
        temp = temp->next;
    }
}

void sortSongs() {
    if (head == nullptr || head->next == nullptr) {
        cout << "\nNot enough songs to sort.\n";
        return;
    }

    bool swapped;
    do {
        swapped = false;
        Song* current = head;
        Song* prev = nullptr;

        while (current->next != nullptr) {
            Song* nextNode = current->next;

            if (current->song.title > nextNode->song.title) {
                swapped = true;

                if (prev == nullptr) {
                    current->next = nextNode->next;
                    nextNode->next = current;
                    head = nextNode;
                    prev = nextNode;
                } else {
                    prev->next = nextNode;
                    current->next = nextNode->next;
                    nextNode->next = current;
                    prev = nextNode;
                }
            } else {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);

    cout << "\nSongs sorted alphabetically.\n";
}

//USES BST
void searchByID();

void removeSong() {
    if (head == nullptr) {
        cout << "\nPlaylist is empty.\n";
        return;
    }

    cout << "\nEnter song title to remove: ";
    string title;
    getLineSafe(title);

    Song* temp = head;
    Song* prev = nullptr;

    while (temp != nullptr && temp->song.title != title) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == nullptr) {
        cout << "\nSong not found.\n";
        return;
    }

    if (prev == nullptr)
        head = temp->next;
    else
        prev->next = temp->next;

    delete temp;
    cout << "\nSong removed.\n";
}

void clearPlaylist() {
    Song* temp;
    while (head != nullptr) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

void hardcodedSongs() {
    addSong("Yo Perreo Sola", "Bad Bunny");
    addSong("Foreword", "Tyler the Creator");
    addSong("You don't know my name", "Alicia Keys");
    addSong("Hotel California", "Eagles");
    addSong("Billie Jean", "Michael Jackson");
    addSong("My Last", "Big Sean");
    addSong("Houston Old Head", "A$AP Rocky");
    addSong("Blinding Lights", "The Weeknd");
    addSong("Some Nights", "Fun");
    addSong("Meet Me Halfway", "Black Eyed Peas");
}

void testBST() {
    Song* song1 = new Song;
    song1->song = SongClass("Song A", "Artist A"); // ID=1
    Song* song2 = new Song;
    song2->song = SongClass("Song B", "Artist B"); // ID=2
    Song* song3 = new Song;
    song3->song = SongClass("Song C", "Artist C"); // ID=3

    SongBST bst;
    bst.insert(song2);   // insert out of order
    bst.insert(song1);
    bst.insert(song3);

    cout << "BST in-order traversal (should be ID 1,2,3):\n";
    bst.inOrder();
}
// PLAY NEXT FEATURE using queue
struct Track {
    string title;
    string artist;

    string toString() const {
        return title + " - " + artist;
    }
};

queue<Track> upNext;
Track currentTrack;
bool hasCurrent = false;

void addToPlayNextByNumber() {
    if (head == nullptr) {
        cout << "\nPlaylist is empty.\n";
        return;
    }

    displaySongs();
    cout << "\nEnter song number: ";
    int n;
    cin >> n;
    cin.ignore();  // remove leftover newline for getline()

    Song* temp = head;
    int count = 1;
    while (temp && count < n) {
        temp = temp->next;
        count++;
    }

    if (!temp) {
        cout << "Invalid number.\n";
        return;
    }

    upNext.push({temp->song.title, temp->song.artist});
    cout << "Added to Play Next.\n";
}

void viewUpNext() {
    if (upNext.empty()) {
        cout << "[Queue empty]\n";
        return;
    }

    cout << "FRONT -> ";
    queue<Track> copy = upNext; // copy to iterate
    bool first = true;
    while (!copy.empty()) {
        if (!first) cout << " | ";
        cout << copy.front().toString();
        copy.pop();
        first = false;
    }
    cout << " <- BACK\n";
}

void clearUpNext() {
    while (!upNext.empty()) upNext.pop();
    cout << "Up Next cleared.\n";
}

void playNext() {
    if (upNext.empty()) {
        hasCurrent = false;
        cout << "Up Next queue empty.\n";
        return;
    }

    currentTrack = upNext.front();
    upNext.pop();
    hasCurrent = true;

    cout << "\n▶ Now Playing: " << currentTrack.toString() << endl;
}

void showCurrent() {
    if (!hasCurrent)
        cout << "Nothing is playing.\n";
    else
        cout << "▶ Currently Playing: " << currentTrack.toString() << endl;
}


// MAIN MENU
int main() {
    hardcodedSongs();
    //testBST();

    int choice;

    while (true) {
        cout << "\n========== MUSIC PLAYLIST MANAGER ==========\n";
        cout << "1. Add Song\n";
        cout << "2. Display Songs\n";
        cout << "3. Sort Songs\n";
        //Include sort by ID w/ BST
        cout << "4. Remove Song\n";
        cout << "5. Add to Play Next (choose from playlist)\n";
        cout << "6. View Up Next\n";
        cout << "7. Play Next\n";
        cout << "8. Clear Up Next\n";
        cout << "9. Show currently playing\n";
        cout << "10. Exit\n";
        cout << "Choose: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Exiting.\n";
            break;
        }
        cin.ignore(); // clear newline so getline works

        switch (choice) {
            case 1: addSongUser(); break;
            case 2: displaySongs(); break;
            case 3: sortSongs(); break;
            case 4: removeSong(); break;
            case 5: addToPlayNextByNumber(); break;
            case 6: viewUpNext(); break;
            case 7: playNext(); break;
            case 8: clearUpNext(); break;
            case 9: showCurrent(); break;
            case 10:
                clearPlaylist();
                cout << "\nGoodbye!\n";
                return 0;
            default:
                cout << "Invalid choice.\n";
        }
    }

    clearPlaylist();
    
    return 0;
}
