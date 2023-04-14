struct Record
{
	char nombre[30];
	char carrera[20];
	int ciclo;
	void setData(){
		
	}
	string getKey(){
		//todo
	}
};	
 
class RandomFile{
private:
	string filename;
	string indexname;
	map<string, long> index;	
public:
	RandomFile(string filename){
		this->filename = filename;
		this->indexname = filename + "ind";
		readIndex();
	}
	
	void write(Record data){
		//TODO
	}
	
	void scanAll(){
		//TODO
	}
	
	//recorrido en O(n) usando el index
	void scanAllByIndex(){
		//TODO
	}
	
	~RandomFile(){
		writeIndex();
	}
};