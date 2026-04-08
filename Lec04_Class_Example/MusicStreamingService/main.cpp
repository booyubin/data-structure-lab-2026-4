#include "Music.h"
using namespace std;
	int main() {
	//streaming Service test
	MusicStreamingService myService("Spotify");
	//음악 Service에 추가하기
	myService.addMusic("SWIM", "BTS", "ARIRANG", 2026);
	myService.addMusic("Rude", "Hearts2Hearts", "Rude", 2026);
	//title로 search
	string music_title;
	cout << "enter th music title: ";
	cin >> music_title;
	//=scanf("%s", &music_title);
	Music* result = myService.searchByTitle(music_title);
	if (result != NULL) {
		cout << "Found: " << result->getTitle() << " by" << result-> getArtist() << endl;
	}
	else {
		cout << "Not Found" << endl;
	}

	string artist_name;
	cout << "Enter the Artist Name: ";
	cin >> artist_name;

	vector<Music*> artistResult = myService.searchByArtist(artist_name);
	if (artistResult.size() > 0) {
		cout << "Found " << artistResult.size() << " songs by " << artist_name << ": " << endl;
		for (int i = 0; i < artistResult.size();i++) {
			cout << artistResult[i]->getTitle() << endl; 
		}
	}
	else {
		cout << " Not Found" << endl;
	}
	return 0;
}