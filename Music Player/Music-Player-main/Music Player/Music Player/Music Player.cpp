#include <iostream>
#include <fstream>
#include <windows.h>
#include<mmsystem.h>
#include <string>
#include <string>
#include <locale>
#include <codecvt>
#include <iomanip>

using namespace std;
void pas() {
    char ch;
    cout << "\t  |";
    system("pause");

    ch = getchar();
    if (ch == '\n') {
        std::cout << "Continuing...";
    }
}
void toppan() {
    cout << " |\t\t\t\t\t\tSong   Singer  genre\t    " << endl;
    cout << " |\t\t\t\t\t\t============================" << endl;
}
void art()
{




    ifstream ifs("art.txt");
    string Lines = "";

    if (ifs)
    {
        while (ifs.good())
        {
            string TempLine;
            getline(ifs, TempLine);
            TempLine += "\n";

            Lines += TempLine;
        }

        cout << Lines;

    }

    ifs.close();
}


wstring stringToWstring(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str) ;
}

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
            cout<< " |\t\t\t\t\t\t "<< current->song->title << "  " << current->song->artist<<"  " << " (" << current->song->genre << ")" << endl;
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
            cout<<" |\t\t\t\t\t\t " << node->title << "\t" << node->artist<<" \t" << " (" << node->genre << ")" << endl;
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
        toppan();
        inorder(root);
    }

    void playQueue() {
        while(!playlist.empty()) {
            Song* nextSong = playlist.getFront(); 
            wstring s = stringToWstring(nextSong->title);
            playlist.pop();
            PlaySound((s+L".wav").c_str(), NULL, SND_ASYNC);
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
            wstring s = stringToWstring(nextSong->title);
            playlist.pop();
            PlaySound((s + L".wav").c_str(), NULL, SND_ASYNC);
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

void controlpan() {
    MusicPlayer player;
    int x;
    string title, artist, genre;
    player.loadFromFile("songs.txt");
   
   
    do {
        system("cls");
        art();
        cout <<"\t\t\t\t\t\t\  1.Add Song \n"
             <<"\t\t\t\t\t\t\  2.Display songs \n"
             <<"\t\t\t\t\t\t\  3.Play a Single Song \n"
             <<"\t\t\t\t\t\t\  4.Add Song to Queue\n"
             <<"\t\t\t\t\t\t\  5.Play Queue \n"
             << "\t\t\t\t\t\t  6.start playing \n"
             <<"\t\t\t\t\t\t\  7.Play Songs by Artist \n"          
             <<"\t\t\t\t\t\t\  8.Play All \n" 
             <<"\t\t\t\t\t\t\  9. Display your Queue \n"
             <<"\t\t\t\t\t\t\  0.Exit" << endl<<endl;
        cout <<"\t\t\t\t\t\t\   you choice ";
        cin >> x;

        if (x == 1)
        {
            system("cls");
            //Add song
            cout << "insert the artist, title, genre \n";
            cin >> artist >> title >> genre;
            player.addSong(title, artist, genre);
        }
        else if (x == 2)
        {
            system("cls");
            art();
            //Displays Songs
            player.displaySongs();
            pas();
        }
        else if (x == 3)
        {    
            system("cls");
            art();
            //Play a Single song
            cout << "Title of the Song: ";
            cin >> title;
            player.addToPlaylist(title);
            player.playNext();

        }
        else if (x == 4)
        {
            system("cls");
            art();
            //Add a song to Queue
            cout << "Title of the Song: ";
            cin >> title;
            player.addToPlaylist(title);
        }
        else if (x == 5)
        {
            system("cls");
            art();
            //Plays all Queue
            player.playQueue();
        }
        else if (x == 6)
        {
            system("cls");
            art();
            //Plays next 
            toppan();
            player.displayQueue();
            player.playNext();
            //Displays Songs in Queue
            char y;
            do{
            cout << "enter n too play next";
            cin >> y;
            system("cls");
            art();
            toppan();
            player.displayQueue();
            player.playNext();
            }while(y=='n');
            
            
        }
        else if (x == 7)
        {
            system("cls");
            //Plays Songs by Certain Artist
            cout << "Enter The Artist you want to listen to: " << endl;
            cin >> artist;
            player.PlaySongsByArtist(artist);
            player.playNext();
        }
        else if (x == 8)
        {
            system("cls");
            //Loads all songs
            player.loadTreeToQueue();
        }
        else if (x == 9)
        {
       
            
        }
      
    } while (x != 0);
    player.saveToFile("songs.txt");

}
int main() {
   
    controlpan();
}
