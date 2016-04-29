#pragma once
#include <vector>

using namespace std;

class Album
{
private:
	string _nome;
	string _descricao;
	string _diretoria;
	string _genero;
	string _imagem;
	vector <Autor>* _autor;
	vector <Musica>* _musica;
public:
	Album();
	~Album();

	string getNome();
	string getDescricao();
	string getDiretoria();
	string getGenero();
	int getAutor(vector <Autor>&);
	string getImagem();
	int getMusica(vector <Musica>&);
	int apagar();
	int play();
	int adicionar();
	//int procurar(string);

	int criar();
	
};

class Playlist
{
private:
	string _nome;
	string _descricao;
	vector <Musica>* _musica;
public:
	Playlist();
	~Playlist();

	string getNome();
	string getDescricao();
	int getMusica(vector <Musica>&);
	int apagar();
	int play();
	int adicionar();
	//int procurar(string);
	int remover();

	int criar();
	
};

class Autor
{
private:
	string _nome;
	string _nacionalidade;
	time_t _dataNascimento;
public:
	Autor();
	~Autor();

	string getNome();
	string getNacionalidade();
	time_t getDataNascimento();
	int apagar();
	int play();
	//int procurar(string);

	int criar();
	
};

class Musica
{
private:
	string _nome;
	string _diretoria;
	vector <Autor>* _autor;
	string _genero;
	int _faixa;
public:
	Musica();
	~Musica();

	string getNome();
	string getDiretoria();
	int getAutor(vector <Autor>&);
	string getGenero();
	int getFaixa();
	int apagar();
	int play();
	//int procurar(string);
	
};

class Player
{
private:
	Playlist* _lista;
public:
	Player();
	~Player();

	int play();
	int pausa();
	int parar();
	int seguinte();
	int anterior();
	int adicionar();
	int remover();
	
};