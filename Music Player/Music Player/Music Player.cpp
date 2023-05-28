#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct Song {
    string title;
    string artist;
    string genre;
    Song* left;
    Song* right;
    Song(string t, string a, string g) : title(t), artist(a), genre(g), left(NULL), right(NULL) {}
};

struct QueueNode {
    Song* song;
    QueueNode* next;
    QueueNode(Song* s) : song(s), next(NULL) {}
};

class Queue {
private:
    QueueNode* front;
    QueueNode* back;

public:
    Queue() : front(NULL), back(NULL) {}

    void push(Song* song) {
        QueueNode* newNode = new QueueNode(song);
        if (front == NULL) {
            front = newNode;
            back = newNode;
        }
        else {
            back->next = newNode;
            back = newNode;
        }
    }

    void pop() {
        if (front != NULL) {
            QueueNode* temp = front;
            front = front->next;
            delete temp;
        }
    }

    Song* getFront() {
        if (front != NULL) {
            return front->song;
        }
        else {
            return NULL;
        }
    }
   
    void display() {
        QueueNode* current = front;
        while (current != NULL) {
            cout << current->song->title << " by " << current->song->artist << " (" << current->song->genre << ")" << endl;
            current = current->next;
        }
    }

    bool empty() {
        return (front == NULL);
    }
};

class MusicPlayer {
private:
    Song* root;
    Queue playlist;

    void insert(Song*& node, string title, string artist, string genre) {
        if (node == NULL) {
            node = new Song(title, artist, genre);
        }
        else if (title < node->title) {
            insert(node->left, title, artist, genre);
        }
        else {
            insert(node->right, title, artist, genre);
        }
    }

    void inorder(Song* node) {
        if (node != NULL) {
            inorder(node->left);
            cout << node->title << " by " << node->artist << " (" << node->genre << ")" << endl;
            inorder(node->right);
        }
    }
    void loadInorder(Song* node) {
        if (node != NULL) {
            loadInorder(node->left);
            playlist.push(node);
            loadInorder(node->right);
        }
    }
    void saveInorder(Song* node, ofstream& file) {
        if (node != NULL) {
            saveInorder(node->left, file);
            file << node->title << " ";
            file << node->artist << " ";
            file << node->genre << endl;
            saveInorder(node->right, file);
        }
    }
    void searchByArtist(Song* node, string artist) {
        if (node != NULL) {
            searchByArtist(node->left, artist);
            if (node->artist == artist) {
                playlist.push(node);
            }
            searchByArtist(node->right, artist);
        }
    }

public:
    MusicPlayer() : root(NULL) {}

    void loadFromFile(string filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string title, artist, genre;
            while (file >> title) {
                file >> artist;
                file >> genre;
                addSong(title, artist, genre);
                
            }
            file.close();
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }
    void saveToFile(string filename) {
        ofstream file(filename);
        if (file.is_open()) {
            saveInorder(root, file);
            file.close();
        }
        else {
            cout << "Unable to open file." << endl;
        }
    }

    

    void addSong(string title, string artist, string genre) {

        insert(root, title, artist, genre);
    }


    void displaySongs() {
        inorder(root);
    }

    void playQueue() {
        while(!playlist.empty()) {
            Song* nextSong = playlist.getFront();
            playlist.pop();
            cout << "Now playing: " << nextSong->title << " by " << nextSong->artist << endl;
        }
        if (playlist.empty())
        {
            cout << "Queue is Empty!" << endl;
        }
       
    }
    void playNext() {
        if (!playlist.empty()) {
            Song* nextSong = playlist.getFront();
            playlist.pop();
            cout << "Now playing: " << nextSong->title << " by " << nextSong->artist << endl;
        }
        else
        {
            cout << "No song is in queue!";
        }

    }

    void addToPlaylist(string title) {
        Song* song = search(root, title);
        if (song != NULL) {
            playlist.push(song);
            cout <<  song->title << " by " << song->artist << " Added to Queue!" << endl;
        }
        else {
            cout << "Song not found!" << endl;
        }
    }
    void loadTreeToQueue() {
        loadInorder(root);
    }

    

    void PlaySongsByArtist(string artist) {
        searchByArtist(root, artist);
    }
   
    void displayQueue() {
        cout << "Queue:" << endl;
        playlist.display();
    }
    

    Song* search(Song* node, string title) {
        if (node == NULL || node->title == title) {
            return node;
        }
        else if (title < node->title) {
            return search(node->left, title);
        }
        else {
            return search(node->right, title);
        }
    }
};

int main() {
    MusicPlayer player;
    int x;
    string title, artist, genre;
    player.loadFromFile("songs.txt");
    cout << " 1.Add Song \n 2.Display songs \n 3.Play a Single Song \n 4.Add Song to Queue \n 5.Play Queue \n 6.Play next \n 7.Play Songs by Certain Artist \n 8.Loads all songs to the Queue \n 9. Display your Queue \n 0.Exit" << endl;
    do {
        cin >> x;

        if (x == 1)
        {
            //Add song
            cout << "insert the artist, title, genre \n";
            cin >> artist >> title >> genre;
            player.addSong(title, artist, genre);
        }
        else if (x == 2)
        {
            //Displays Songs
            player.displaySongs();
        }
        else if (x == 3)
        {
            //Play a Single song
            cout << "Title of the Song: ";
            cin >> title;
            player.addToPlaylist(title);
            player.playNext();

        }
        else if (x == 4)
        {
            //Add a song to Queue
            cout << "Title of the Song: ";
            cin >> title;
            player.addToPlaylist(title);
        }
        else if (x == 5)
        {
            //Plays all Queue
            player.playQueue();
        }
        else if (x == 6)
        {
            //Plays next 
            player.playNext();
        }
        else if (x == 7)
        {
            //Plays Songs by Certain Artist
            cout << "Enter The Artist you want to listen to: " << endl;
            cin >> artist;
            player.PlaySongsByArtist(artist);
            player.playNext();
        }
        else if (x == 8)
        {
            //Loads all songs
            player.loadTreeToQueue();
        }
        else if (x == 9)
        {
            //Displays Songs in Queue
            player.displayQueue();
            
        }
      
    } while (x != 0);
    player.saveToFile("songs.txt");

}