#include <fstream>
#include <iostream>
#include <map>
#include <cstring>
struct Record
{
	char nombre[30];
	char carrera[20];
	int ciclo;
	void setData(char n[], char c[], int ci){
        strncpy(nombre, n, sizeof(nombre));
        strncpy(carrera, c, sizeof(carrera));
        ciclo = ci;
	}
	std::string getKey(){
        return std::string(nombre) + "-" + std::string(carrera) + "-" + std::to_string(ciclo);
    }
};	
 
class RandomFile{
private:
	std::string filename;
	std::string indexname;
	std::map<std::string, long> index;
    void readIndex(){
        // abrir archivo de índice en modo de lectura binaria
        std::ifstream indexfile(indexname.c_str(), std::ios::binary);

        // leer cada entrada del archivo de índice y agregarla al mapa
        IndexEntry entry{};
        while(indexfile.read((char*)&entry, sizeof(entry))){
            index[std::string(entry.key)] = entry.value;
        }

        // cerrar el archivo
        indexfile.close();
    }
public:
	explicit RandomFile(const std::string& filename){
		this->filename = filename;
		this->indexname = filename + "ind";
		readIndex();
	}
	
	void write(Record data){
        // abrir archivo en modo de escritura binaria
        std::ofstream outfile(filename.c_str(), std::ios::binary | std::ios::app);

        // escribir el registro en el archivo
        outfile.write((char*)&data, sizeof(data));

        // actualizar el índice con la nueva ubicación del registro
        index[data.getKey()] = static_cast<size_t>(outfile.tellp()) - sizeof(data);
        // cerrar el archivo
        outfile.close();
    }
	
	void scanAll(){
        // abrir archivo en modo de lectura binaria
        std::ifstream infile(filename.c_str(), std::ios::binary);

        // leer y mostrar todos los registros del archivo
        Record record{};
        while(infile.read((char*)&record, sizeof(record))){
            std::cout << "Nombre: " << record.nombre << ", Carrera: " << record.carrera << ", Ciclo: " << record.ciclo << std::endl;
        }

        // cerrar el archivo
        infile.close();
    }
	
	//recorrido en O(n) usando el index
    void scanAllByIndex(){
        // abrir el archivo de datos en modo de lectura binaria
        std::ifstream datafile(filename.c_str(), std::ios::binary);

        // recorrer el mapa de índice y buscar cada registro en el archivo de datos
        for(auto & it : index){
            // ir a la posición del registro en el archivo de datos
            datafile.seekg(it.second, std::ios::beg);

            // leer el registro desde el archivo de datos
            Record record{};
            datafile.read((char*)&record, sizeof(record));

            // mostrar el contenido del registro
            std::cout << "Nombre: " << record.nombre << ", Carrera: " << record.carrera << ", Ciclo: " << record.ciclo << std::endl;
        }

        // cerrar el archivo de datos
        datafile.close();
    }

	
	~RandomFile(){
		writeIndex();
	}
    struct IndexEntry {
        char key[30];
        long value;
    };
// escribir el índice en el archivo
    void writeIndex(){
        // abrir archivo de índice en modo de escritura binaria
        std::ofstream indexfile(indexname.c_str(), std::ios::binary);

        // recorrer el mapa y escribir cada entrada en el archivo de índice
        for(auto & it : index){
            IndexEntry entry{};
            strncpy(entry.key, it.first.c_str(), sizeof(entry.key));
            entry.value = it.second;
            indexfile.write((char*)&entry, sizeof(entry));
        }

        // cerrar el archivo
        indexfile.close();
    }

};
